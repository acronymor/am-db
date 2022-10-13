#include "sql/storage/bplus_tree_node.h"

#include "common/assert.h"
#include "sql/codec/codec.h"

namespace amdb {
namespace storage {
DEFINE_uint32(bptree_max_node_size, 16 * (1 << 10), "the size of bptree node");

TreeCtx::TreeCtx(amdb::Arena* arena, KvStorageAPI* kv)
    : arena_(arena), storage_api_(kv) {}

Arena* TreeCtx::AllocMem() { return arena_; }

uint64_t TreeCtx::AllocateNodeID() { return id_++; }

void TreeCtx::CollectUnsavedTreeNode(BptNode* node) {
  unsaved_nodes_[node->ID()] = node;
}

void TreeCtx::RemoveUnsavedTreeNode(BptNode* node) {
  unsaved_nodes_.erase(node->ID());
}

Status TreeCtx::GetNodeFromKVStorage(uint64_t node_id, std::string* value) {
  // std::string key = GetBTreeNodeKey(node_id);
  std::string key = std::to_string(node_id);
  return storage_api_->GetKV(key, value);
}

void TreeCtx::PullUnsavedTreeNode(std::vector<std::string>* keys,
                                  std::vector<std::string>* values) {
  keys->reserve(unsaved_nodes_.size());
  values->reserve(unsaved_nodes_.size());

  for (auto& entry : unsaved_nodes_) {
    std::string value;
    entry.second->Serialize(&value);
    keys->emplace_back(std::move(std::to_string(entry.first)));
    values->emplace_back(std::move(value));
  }

  unsaved_nodes_.clear();
}

BptNode::BptNode(TreeCtx* ctx, BptNonLeafNodeProto* root) {
  tree_ctx_ = ctx;

  id_ = root->id();
  is_loaded_ = true;
  is_root_ = true;
  is_leaf_ = false;
  parent_ = nullptr;
  children_.reserve(root->children_size());

  stat_.count = 0;
  stat_.node_size = 0;

  for (int i = 0; i < root->children_size(); i++) {
    BptNode* node = tree_ctx_->AllocMem()->CreateObject<BptNode>(
        tree_ctx_, this, root->children(i));
    stat_.count += node->stat_.count;
    // 20 = fixed size of encode key
    stat_.node_size +=
        node->stat_.max_key.size() + node->stat_.min_key.size() + 20;
    children_.emplace_back(node);
  }

  if (!children_.empty()) {
    stat_.min_key = children_.front()->stat_.min_key;
    stat_.max_key = children_.back()->stat_.max_key;
  } else {
    stat_.min_key.clear();
    stat_.max_key.clear();
  }
}

BptNode::BptNode(TreeCtx* ctx, BptNode* parent, const BptNodeRefProto& node) {
  tree_ctx_ = ctx;

  id_ = node.id();
  is_loaded_ = true;
  is_leaf_ = node.type() == BptNodeRefProto::LEAF;
  is_root_ = false;

  parent_ = parent;
  stat_.count = node.stat().count();
  stat_.max_key = node.stat().max_key();
  stat_.min_key = node.stat().min_key();
  stat_.node_size = node.stat().node_size();
}

BptNode::BptNode(TreeCtx* ctx, BptNode* left, BptNode* right) {
  tree_ctx_ = ctx;

  id_ = tree_ctx_->AllocateNodeID();

  left->is_root_ = false;
  left->parent_ = this;
  right->is_root_ = false;
  right->parent_ = this;

  is_loaded_ = true;
  is_leaf_ = false;
  is_root_ = true;
  parent_ = nullptr;

  children_.emplace_back(left);
  children_.emplace_back(right);

  stat_.count = left->stat_.count + right->stat_.count;
  stat_.max_key = right->stat_.max_key;
  stat_.min_key = left->stat_.min_key;
  stat_.node_size = left->stat_.min_key.size() + left->stat_.max_key.size() +
                    20 + right->stat_.min_key.size() +
                    right->stat_.max_key.size() + 20;
}

BptNode::BptNode(TreeCtx* ctx, BptNode* parent, bool is_leaf) {
  tree_ctx_ = ctx;
  id_ = tree_ctx_->AllocateNodeID();

  is_loaded_ = true;
  is_leaf_ = is_leaf;
  is_root_ = false;
  parent_ = parent;
}

BptNode::~BptNode() {
  for (auto& child : children_) {
    child = nullptr;
  }
  children_.clear();
}

Status BptNode::Insert(const std::string& key, std::string&& value) {
  AMDB_ASSERT_TRUE(is_leaf_);

  if ((kvs_.find(key) != kvs_.end())) {
    ERROR("Duplicate key: {}", Status::C_BPTREE_DUPLICATE_KEY)
    return Status::C_BPTREE_DUPLICATE_KEY;
  }

  auto result = write_kvs_.emplace(key, value);
  if (!result.second) {
    ERROR("BptNode Insert Failed: {}", Status::C_BPTREE_ERROR);
    return Status::C_BPTREE_ERROR;
  }
  auto& it = result.first;
  kvs_.emplace(it->first, it->second);
  return Status::C_OK;
}

Status BptNode::Update(const std::string& key, std::string& value) {
  return Status::C_OK;
}

Status BptNode::Delete(const std::string& key) {
  AMDB_ASSERT_TRUE(is_leaf_);
  auto it = kvs_.find(key);
  if (it != kvs_.end()) {
    kvs_.erase(it);
    write_kvs_.erase(key);
    return Status::C_OK;
  }

  return Status::C_BPTREE_ERROR;
}

Status BptNode::GetItem(const std::string& key, std::string* value) {
  auto it = kvs_.find(key);
  if (it == kvs_.end()) {
    return Status::C_BPTREE_ERROR;
  } else {
    *value = it->second;
    return Status::C_OK;
  }
}

uint64_t BptNode::ID() const { return id_; }

bool BptNode::IsLeaf() const { return is_leaf_; }

bool BptNode::IsRoot() const { return is_root_; }

BptNode* BptNode::Parent() const { return parent_; }

bool BptNode::IsEmpty() const { return children_.empty() && kvs_.empty(); }

void BptNode::AddChild(BptNode* child_node) {
  auto it = MaxKeyUpperBound(child_node->stat_.max_key);
  children_.insert(it, child_node);
}

void BptNode::DelChild(BptNode* child_node) {
  auto it = FindChild(child_node);
  if (it != children_.cend()) {
    children_.erase(it);
    delete child_node, child_node = nullptr;
  }
}

struct CompareMaxKey {
  bool operator()(const BptNode* s1, const BptNode* s2) {
    return IsCmpLe(DataCmp(s1->Stat().max_key, s2->Stat().max_key));
  }
};

ChildIt BptNode::MaxKeyLowerBound(const std::string& key) const {
  BptNode node;
  node.stat_.max_key = key;

  return std::lower_bound(children_.cbegin(), children_.cend(), &node,
                          CompareMaxKey());
}

ChildIt BptNode::MaxKeyUpperBound(const std::string& key) const {
  BptNode node;
  node.stat_.max_key = key;
  ChildIt it = std::upper_bound(children_.cbegin(), children_.cend(), &node,
                                CompareMaxKey());
  return it;
}

struct CompareMinKey {
  bool operator()(const BptNode* s1, const BptNode* s2) {
    return IsCmpLe(DataCmp(s1->Stat().min_key, s2->Stat().min_key));
  }
};

ChildIt BptNode::MinKeyLowerBound(const std::string& key) const {
  BptNode node;
  node.stat_.min_key = key;
  return std::lower_bound(children_.cbegin(), children_.cend(), &node,
                          CompareMinKey());
}

ChildIt BptNode::MinKeyUpperBound(const std::string& key) const {
  BptNode node;
  node.stat_.min_key = key;
  ChildIt it = std::upper_bound(children_.cbegin(), children_.cend(), &node,
                                CompareMinKey());
  return it;
}

ChildIt BptNode::FindChild(const std::string& key) const {
  auto it = MaxKeyLowerBound(key);
  if (it != children_.cend() && IsCmpGe(DataCmp(key, (*it)->stat_.min_key))) {
    return it;
  } else {
    return children_.cend();
  }
}

ChildIt BptNode::FindChild(BptNode* child_node) const {
  auto it = MaxKeyLowerBound(child_node->stat_.max_key);
  if (it != children_.cend() && *it == child_node) {
    return it;
  } else {
    return children_.cend();
  }
}

bool BptNode::HasChild() const { return !children_.empty(); }

Status BptNode::LoadNodeFromKVStorage() {
  std::string value;
  Status status = tree_ctx_->GetNodeFromKVStorage(id_, &value);
  if (status != Status::C_OK) {
    return status;
  }
  Deserialize(value);
  is_loaded_ = true;
  return Status::C_OK;
}

BptNode* BptNode::NewMutableLeafChild() {
  auto child =
      tree_ctx_->AllocMem()->CreateObject<BptNode>(tree_ctx_, this, true);
  children_.emplace_back(child);
  return child;
}

Status BptNode::Serialize(std::string* output) {
  if (is_leaf_) {
    auto leaf_node = BptLeafNodeProto();
    leaf_node.set_id(id_);
    std::string* keys = leaf_node.mutable_keys();
    std::string* values = leaf_node.mutable_values();
    for (auto& kv : kvs_) {
      amdb::codec::EncodeBytes(kv.first, keys);
      amdb::codec::EncodeBytes(kv.second, values);
    }
    *output = leaf_node.SerializeAsString();
  } else {
    auto non_leaf_node = BptNonLeafNodeProto();
    non_leaf_node.set_id(id_);
    for (auto& child : children_) {
      auto child_proto = non_leaf_node.add_children();
      child_proto->set_id(child->ID());
      child_proto->set_type(child->IsLeaf() ? BptNodeRefProto::LEAF
                                            : BptNodeRefProto::NON_LEAF);
      child_proto->mutable_stat()->set_count(child->stat_.count);
      child_proto->mutable_stat()->set_max_key(child->stat_.max_key);
      child_proto->mutable_stat()->set_min_key(child->stat_.min_key);
      child_proto->mutable_stat()->set_node_size(child->stat_.node_size);
    }
    *output = non_leaf_node.SerializeAsString();
  }

  return Status::C_OK;
}

Status BptNode::Deserialize(std::string& input) {
  if (is_leaf_) {
    bool success = proto_.ParseFromString(input);

    auto& keys = proto_.keys();
    auto& values = proto_.values();

    uint64_t k_offset = 0;
    uint64_t v_offset = 0;
    uint32_t k_len = 0;
    uint32_t v_len = 0;

    while (k_offset < keys.length()) {
      k_offset += amdb::codec::DecodeCmpUInt32(
          std::string(keys.data() + k_offset, keys.size()), &k_len);
      v_offset += amdb::codec::DecodeCmpUInt32(
          std::string(values.data() + v_offset, values.size()), &v_len);
      kvs_.emplace(std::string(keys.data() + k_offset, k_len),
                   std::string(values.data() + v_offset, v_len));
      k_offset += k_len;
      v_offset += v_len;
    }
  } else {
    BptNonLeafNodeProto non_leaf;
    bool success = non_leaf.ParseFromString(input);
    std::cout << "Deserialize[non_leaf]: => " << success << std::endl;

    children_.reserve(non_leaf.children_size());
    for (const auto& child : non_leaf.children()) {
      BptNode* child_node =
          tree_ctx_->AllocMem()->CreateObject<BptNode>(tree_ctx_, this, child);
      children_.emplace_back(child_node);
    }
  }

  return Status::C_OK;
}

const NodeStatistics& BptNode::Stat() const { return stat_; }

void BptNode::UpdateStatByAddKV(const std::string& key,
                                const std::string& value) {
  stat_.count = kvs_.size();
  stat_.node_size = stat_.node_size + value.size();

  if (stat_.count == 1) {
    stat_.max_key = stat_.min_key = key;
    stat_.node_size += (stat_.max_key.size() + stat_.min_key.size() + 20);
    return;
  }

  if (IsCmpGt(DataCmp(key, stat_.max_key))) {
    stat_.node_size = stat_.node_size - stat_.max_key.size() + key.size();
    stat_.max_key = key;
  }

  if (IsCmpLt(DataCmp(key, stat_.min_key))) {
    stat_.node_size = stat_.node_size - stat_.min_key.size() + key.size();
    stat_.min_key = key;
    return;
  }
}

void BptNode::UpdateStatByDelKV(const std::string& key) {
  stat_.count = kvs_.size();

  if (IsCmpEq(DataCmp(key, stat_.max_key))) {
    stat_.node_size = stat_.node_size - stat_.max_key.size();
    stat_.max_key =
        std::string(kvs_.rbegin()->first.data(), kvs_.rbegin()->first.size());
    return;
  }

  if (IsCmpEq(DataCmp(key, stat_.min_key))) {
    stat_.node_size = stat_.node_size - stat_.min_key.size();
    stat_.min_key =
        std::string(kvs_.begin()->first.data(), kvs_.begin()->first.size());
    return;
  }
}

void BptNode::UpdateStatByRangeChange(bool is_add) {
  std::string* min_key = &children_.front()->stat_.min_key;
  if (IsCmpNe(DataCmp(*min_key, stat_.min_key))) {
    stat_.node_size = stat_.node_size - stat_.min_key.size() + min_key->size();
    stat_.min_key = *min_key;
  }

  std::string* max_key = &children_.back()->stat_.max_key;
  if (IsCmpNe(DataCmp(*max_key, stat_.max_key))) {
    stat_.node_size = stat_.node_size - stat_.max_key.size() + max_key->size();
    stat_.max_key = *max_key;
  }

  stat_.count += is_add ? 1 : -1;
}

void BptNode::ResetStat() {
  stat_.count = 0;
  stat_.node_size = 0;
  stat_.min_key.clear();
  stat_.max_key.clear();
}

bool BptNode::NeedSplit() const {
  if (is_leaf_) {
    if (kvs_.size() < 3) {
      return false;
    }
  } else {
    if (children_.size() < 3) {
      return false;
    }
  }
  return stat_.node_size > FLAGS_bptree_max_node_size;
}

BptNode* BptNode::Split() {
  BptNode* right_node = tree_ctx_->AllocMem()->CreateObject<BptNode>(
      tree_ctx_, parent_, is_leaf_);
  size_t split_idx = ((kvs_.size() + 1) >> 1) - 1;
  if (is_leaf_) {
    auto iter = kvs_.begin();

    std::advance(iter, split_idx + 1);
    while (iter != kvs_.end()) {
      right_node->stat_.node_size += iter->first.size() + iter->second.size();
      std::string key = iter->first;
      std::string value = iter->second;
      right_node->Insert(std::move(key), std::move(value));
      write_kvs_.erase(iter->first);
      iter = kvs_.erase(iter);
    }

    // update stat
    stat_.count = kvs_.size();
    stat_.min_key = kvs_.begin()->first;
    stat_.max_key = kvs_.rbegin()->first;
    stat_.node_size = stat_.node_size - right_node->stat_.node_size;

    right_node->stat_.count = right_node->stat_.count;
    right_node->stat_.min_key = right_node->kvs_.begin()->first;
    right_node->stat_.max_key = right_node->kvs_.rbegin()->first;
  } else {
    for (size_t i = split_idx + 1; i < children_.size(); i++) {
      BptNode* child = children_[i];
      child->parent_ = right_node;
      right_node->children_.emplace_back(child);

      right_node->stat_.count += child->stat_.count;
      right_node->stat_.node_size +=
          child->stat_.min_key.size() + child->stat_.max_key.size() + 20;
    }

    children_.erase(children_.begin() + split_idx + 1, children_.end());

    stat_.count = stat_.count - right_node->stat_.count;
    stat_.node_size = stat_.node_size - right_node->stat_.node_size;
    stat_.min_key = children_.front()->stat_.min_key;
    stat_.max_key = children_.back()->stat_.max_key;

    right_node->stat_.min_key = right_node->children_.front()->stat_.min_key;
    right_node->stat_.max_key = right_node->children_.back()->stat_.max_key;
  }

  return right_node;
}

}  // namespace storage
}  // namespace amdb