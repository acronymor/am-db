#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "proto/bptree.pb.h"
#include "sql/common/arena.h"
#include "sql/common/statuscode.h"
#include "sql/storage/bplus_tree_cmp.h"
#include "sql/storage/bplus_tree_statistics.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class BptNode;

class TreeCtx {
 public:
  TreeCtx(Arena* arena, KvStorageAPI* kv);

  Arena* AllocMem();

  uint64_t AllocateNodeID();

  void CollectUnsavedTreeNode(BptNode* node);

  void RemoveUnsavedTreeNode(BptNode* node);

  void PullUnsavedTreeNode(std::vector<std::string>* keys,
                           std::vector<std::string>* values);

  Status GetNodeFromKVStorage(uint64_t node_id, std::string* value);

 private:
  // <node_id, BptNode>
  std::unordered_map<uint64_t, BptNode*> unsaved_nodes_;
  uint64_t id_ = 0;

  KvStorageAPI* storage_api_{nullptr};

  Arena* arena_{nullptr};
};

using ChildIt = std::vector<BptNode*>::const_iterator;

class BptNode {
 public:
  ~BptNode();

  BptNode(TreeCtx* ctx, BptNonLeafNodeProto* root);
  BptNode(TreeCtx* ctx, BptNode* parent, const BptNodeRefProto& proto);
  BptNode(TreeCtx* ctx, BptNode* left, BptNode* right);
  BptNode(TreeCtx* ctx, BptNode* parent, bool is_leaf);

  uint64_t ID() const;
  bool IsLeaf() const;
  bool IsRoot() const;
  BptNode* Parent() const;
  bool IsEmpty() const;

  // add/del/update/get
  Status Insert(const std::string& key, std::string&& value);
  Status Update(const std::string& key, std::string& value);
  Status Delete(const std::string& key);
  Status GetItem(const std::string& key, std::string* value);

  // child operate
  void AddChild(BptNode* node);
  void DelChild(BptNode* node);
  bool HasChild() const;

  // find node bound
  ChildIt MaxKeyLowerBound(const BptNode* node) const;
  ChildIt MaxKeyUpperBound(const BptNode* node) const;
  ChildIt MinKeyLowerBound(const BptNode* node) const;
  ChildIt MinKeyUpperBound(const BptNode* node) const;

  // find node
  ChildIt FindChild(const std::string& key) const;
  ChildIt FindChild(BptNode* child_node) const;

  Status LoadNodeFromKVStorage(TreeCtx* ctx);

  // serialize and deserialize
  Status Serialize(std::string* output);
  Status Deserialize(std::string& input);

  // statistics
  const NodeStatistics& Stat() const;
  void UpdateStatByAddKV(const std::string& key, const std::string& value);
  void UpdateStatByDelKV(const std::string& key);
  void UpdateStatByRangeChange(bool is_add);
  void ResetStat();

  bool NeedSplit() const;
  BptNode* Split(TreeCtx* ctx);

  BptNode* parent_ = nullptr;
  // only leaf node has prev_ and next
  BptNode* next_ = nullptr;
  BptNode* prev_ = nullptr;
  // only non-leaf node has children_
  std::vector<BptNode*> children_;

  NodeStatistics stat_ = {};

  BptNode* NewMutableLeafChild();
  // crate temporary nodes, it is not guaranteed that it is always correct
  static BptNode* NewTempNode();

 private:
  BptNode() = default;

  uint64_t id_ = 0;
  bool is_loaded_ = false;
  bool is_leaf_ = false;
  bool is_root_ = false;

  struct cmp_key {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
      return IsCmpLt(DataCmp(lhs, rhs));
    }
  };

  std::map<std::string, std::string, cmp_key> kvs_;
  // store insert key-value
  std::unordered_map<std::string, std::string> write_kvs_;

  BptLeafNodeProto proto_;
  TreeCtx* tree_ctx_;
};
}  // namespace storage
}  // namespace amdb