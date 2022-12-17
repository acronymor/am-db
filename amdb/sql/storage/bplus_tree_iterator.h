#pragma once

#include <queue>

#include "sql/common/statuscode.h"
#include "sql/storage/bplus_tree.h"

namespace amdb {
namespace storage {
/**
 * (lower_open=true, upper_open=true)
 * (lower_open=true, upper_open=false]
 * [lower_open=false, upper_open=true)
 * [lower_open=false, upper_open=false]
 */
struct IteratorOptions {
  std::string lower;
  std::string upper;
  bool lower_open;
  bool upper_open;
};

class BptIterator {
 public:
  explicit BptIterator(Bptree* tree, const IteratorOptions* it_ops)
      : bptree_(tree), cur_node_(bptree_->Root()), it_opts_(it_ops){};

  ~BptIterator() = default;

  Status Open();

  bool HashNext();

  Status Next(std::vector<std::pair<std::string, std::string>>* data_segment, size_t row_count);

 private:
  Bptree* bptree_{nullptr};
  BptNode* cur_node_{nullptr};

  std::queue<BptNode*> using_leaf_node_;
  std::map<std::string, std::string>::iterator cur_kv_;

  const IteratorOptions* it_opts_;

  bool reachRangeBound() const;
  void resetCurKV();
  void nextKV();
  Status nextLeafNode();

  bool node_it_reach_end_ = false;
  bool kv_it_reach_end_ = false;
};
}  // namespace storage
}  // namespace amdb