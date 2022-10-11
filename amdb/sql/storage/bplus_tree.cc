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

      Status status = child->LoadNodeFromKVStorage(ctx);
      if (status != Status::C_OK && status != Status::C_STORAGE_KV_NOT_FOUND) {
        return status;
      }
    } else {
      child = cursor->NewMutableLeafChild(ctx);
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
    if (cursor->IsLeaf()) {
      cursor->UpdateStatByAddKV(key, value);
    } else {
      cursor->UpdateStatByRangeChange(true);
    }

    if (!cursor->NeedSplit()) {
      ctx->CollectUnsavedTreeNode(cursor);
      continue;
    }

    BptNode* new_node = cursor->Split(ctx);
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
      RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(ctx));
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
      RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage(ctx));
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
}  // namespace storage
}  // namespace amdb