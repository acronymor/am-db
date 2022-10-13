#include "sql/storage/bplus_tree.h"

#include <stack>

#include "sql/common/statuscode.h"

namespace amdb {
namespace storage {

Bptree::Bptree(TreeCtx* ctx, BptNonLeafNodeProto* node) : tree_ctx_(ctx) {
  root_ = tree_ctx_->AllocMem()->CreateObject<BptNode>(tree_ctx_, node);
}

Status Bptree::Insert(std::string&& key, std::string&& value) {
  auto cursor = root_;
  std::stack<BptNode*> path;
  path.push(root_);
  // top-down search for the leaf node to insert
  while (!cursor->IsLeaf()) {
    BptNode* child = nullptr;
    if (cursor->HasChild()) {
      auto it = cursor->MaxKeyLowerBound(key);
      child = it == cursor->children_.cend() ? cursor->children_.back() : *it;

      Status status = child->LoadNodeFromKVStorage(tree_ctx_);
      if (status != Status::C_OK && status != Status::C_STORAGE_KV_NOT_FOUND) {
        return status;
      }
    } else {
      child = cursor->NewMutableLeafChild();
    }
    cursor = child;
    path.push(cursor);
  }

  Status status = path.top()->Insert(std::move(key), std::move(value));
  if (status != Status::C_OK) {
    return status;
  }

  while (!path.empty()) {
    cursor = path.top();
    path.pop();
    // update stat
    cursor->IsLeaf() ? cursor->UpdateStatByAddKV(key, value)
                     : cursor->UpdateStatByRangeChange(true);

    if (!cursor->NeedSplit()) {
      tree_ctx_->CollectUnsavedTreeNode(cursor);
      continue;
    }

    BptNode* new_node = cursor->Split(tree_ctx_);
    tree_ctx_->CollectUnsavedTreeNode(new_node);
    tree_ctx_->CollectUnsavedTreeNode(cursor);

    if (cursor->IsRoot()) {
      root_ = tree_ctx_->AllocMem()->CreateObject<BptNode>(tree_ctx_, cursor,
                                                           new_node);
      tree_ctx_->CollectUnsavedTreeNode(root_);
    } else {
      cursor->Parent()->AddChild(new_node);
    }
  }
  return Status::C_OK;
}

Status Bptree::Delete(const std::string& key) {
  auto cursor = root_;
  std::stack<BptNode*> path;
  path.push(root_);
  // top-down search for the leaf node to delete
  while (!cursor->IsLeaf()) {
    BptNode* child = nullptr;
    auto it = cursor->FindChild(key);
    if (it != cursor->children_.cend()) {
      child = *it;
      RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(tree_ctx_));
    }

    if (child == nullptr) {
      return Status::C_OK;
    }
    cursor = child;
    path.push(cursor);
  }

  // delete from the leaf node
  Status status = path.top()->Delete(key);
  if (status != Status::C_OK) {
    return Status::C_OK;
  }

  while (!path.empty()) {
    cursor = path.top();
    path.pop();
    if (!cursor->IsRoot() && cursor->IsEmpty()) {
      tree_ctx_->RemoveUnsavedTreeNode(cursor);
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
    tree_ctx_->CollectUnsavedTreeNode(cursor);
  }
  return Status::C_OK;
}

Status Bptree::GetItem(const std::string& key, std::string* output) const {
  auto cursor = root_;
  // top-down search for the leaf node to get specific item
  while (!cursor->IsLeaf()) {
    BptNode* child = nullptr;
    auto it = cursor->FindChild(key);
    if (it != cursor->children_.cend()) {
      child = *it;
      RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(tree_ctx_));
    }
    if (child == nullptr) {
      ERROR("Not Found key: {}", Status::C_STORAGE_KV_NOT_FOUND);
      return Status::C_STORAGE_KV_NOT_FOUND;
    }
    cursor = child;
  }

  Status status = cursor->GetItem(key, output);
  return status;
}

BptNode* Bptree::Root() { return root_; }

}  // namespace storage
}  // namespace amdb