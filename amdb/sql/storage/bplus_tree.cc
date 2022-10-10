#include "sql/storage/bplus_tree.h"

#include <stack>

#include "sql/common/statuscode.h"

namespace amdb {
namespace storage {

Bptree::Bptree(BptNonLeafNodeProto* node) { root_ = new BptNode(node); }

Status Bptree::Insert(TreeCtx* ctx, std::string&& key, std::string&& value) {
  auto cursor = root_;
  std::stack<BptNode*> path;
  path.push(root_);
  // top-down search for the leaf node to insert
  while (!cursor->IsLeaf()) {
    BptNode* child = nullptr;
    if (cursor->HasChild()) {
      auto it = cursor->MaxKeyLowerBound(key);
      if (it == cursor->children_.cend()) {
        child = cursor->children_.back();
      } else {
        child = *it;
      }
      // RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(ctx));
      RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(ctx));
    } else {
      child = cursor->NewMutableLeafChild(ctx);
    }
    cursor = child;
    path.push(cursor);
  }

  int64_t kv_size = key.size() + value.size();
  bool success = path.top()->Insert(std::move(key), std::move(value));

  IncrStatistics incr_stat;
  while (!path.empty()) {
    cursor = path.top();
    path.pop();
    // No matter the node is split or not, we can incrementally update stat_.
    if (cursor->IsLeaf()) {
      // incrementally update stat_ of leaf node by insert kv.
      incr_stat.node_size = kv_size;
      cursor->UpdateStatByAddKV(key, value);
    } else {
      // incrementally update stat_ of non-leaf node by leaf node status.
      incr_stat.node_size = 0;
      cursor->UpdateStatByRangeChange(true);
    }

    if (!cursor->NeedSplit()) {
      ctx->CollectUnsavedTreeNode(cursor);
      continue;
    }

    IncrStatistics split_change_stat;
    BptNode* new_node = cursor->Split(ctx, &split_change_stat);
    incr_stat.Add(split_change_stat);
    ctx->CollectUnsavedTreeNode(new_node);
    if (cursor->IsRoot()) {
      root_ = new BptNode(ctx, cursor, new_node);
      ctx->CollectUnsavedTreeNode(cursor);
      ctx->CollectUnsavedTreeNode(root_);
    } else {
      cursor->Parent()->AddChild(new_node);
      ctx->CollectUnsavedTreeNode(cursor);
    }
  }
  return Status::C_OK;
}

Status Bptree::Delete(TreeCtx* ctx, const std::string& key) {
  auto cursor = root_;
  std::stack<BptNode*> path;
  path.push(root_);
  // top-down search for the leaf node to delete
  while (!cursor->IsLeaf()) {
    BptNode* child = nullptr;
    auto it = cursor->FindChild(key);
    if (it != cursor->children_.cend()) {
      child = *it;
      // RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(ctx));
      child->LoadNodeFromKVStorage(ctx);
    }

    if (child == nullptr) {
      return Status::C_OK;
    }
    cursor = child;
    path.push(cursor);
  }

  // delete from the leaf node
  bool success = path.top()->Delete(key);
  if (!success) {
    return Status::C_OK;
  }

  while (!path.empty()) {
    cursor = path.top();
    path.pop();
    if (!cursor->IsRoot() && cursor->IsEmpty()) {
      ctx->RemoveUnsavedTreeNode(cursor);
      cursor->Parent()->DelChild(cursor);
      continue;
    }
    if (cursor->IsLeaf()) {
      cursor->UpdateStatByDelKV(key);
    } else {
      if (cursor->IsRoot() && cursor->IsEmpty()) {
        cursor->ResetStat();
      } else {
        cursor->UpdateStatByRangeChange(false);
      }
    }
    ctx->CollectUnsavedTreeNode(cursor);
  }
  return Status::C_OK;
}

Status Bptree::GetItem(TreeCtx* ctx, const std::string& key,
                       std::string* output) const {
  auto cursor = root_;
  // top-down search for the leaf node to get specific item
  while (!cursor->IsLeaf()) {
    BptNode* child = nullptr;
    auto it = cursor->FindChild(key);
    if (it != cursor->children_.cend()) {
      child = *it;
      // RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(ctx));
      child->LoadNodeFromKVStorage(ctx);
    }
    if (child == nullptr) {
      return Status::C_BPTREE_ERROR;
    }
    cursor = child;
  }

  bool success = cursor->GetItem(key, output);
  return Status::C_OK;
}
}  // namespace storage
}  // namespace amdb