#include "sql/storage/bplus_tree_iterator.h"

namespace amdb {
namespace storage {
Status BptIterator::Open() {
  while (!cur_node_->IsLeaf()) {
    if (!cur_node_->HasChild()) {
      DEBUG("current node has no child, id={}", cur_node_->ID());
      return Status::C_BPTREE_ITERATOR_END;
    }

    BptNode* child = nullptr;
    ChildIt it;

    it = it_opts_->lower_open ? cur_node_->MaxKeyUpperBound(it_opts_->lower)
                              : cur_node_->MaxKeyLowerBound(it_opts_->lower);

    if (it != cur_node_->children_.cend()) {
      child = *it;
    }

    if (child == nullptr) {
      DEBUG("Failed to find child, node id={}", cur_node_->ID());
      return Status::C_BPTREE_ITERATOR_END;
    }

    RETURN_ERR_NOT_OK(child->LoadNodeFromKVStorage());
    cur_node_ = child;
  }
  using_leaf_node_.push(cur_node_);

  cur_kv_ = it_opts_->lower_open ? cur_node_->kvs_.upper_bound(it_opts_->lower)
                                 : cur_node_->kvs_.lower_bound(it_opts_->lower);

  kv_it_reach_end_ = false;
  node_it_reach_end_ = false;
  return Status::C_OK;
}

bool BptIterator::HashNext() {
  if (kv_it_reach_end_) {
    return !node_it_reach_end_;
  }

  return !reachRangeBound();
}

Status BptIterator::Next(
    std::vector<std::pair<std::string, std::string>>* data_segment,
    size_t row_count) {
  uint64_t rows_remain = row_count;

  while (rows_remain > 0) {
    if (!kv_it_reach_end_ && reachRangeBound()) {
      DEBUG("kv_it_ reach index range bound");
      return Status::C_OK;
    }

    if (!kv_it_reach_end_) {
      data_segment->emplace_back(cur_kv_->first, cur_kv_->second);
      rows_remain--;
      nextKV();
    } else {
      DEBUG("kv_it_ reach end");

      if (node_it_reach_end_) {
        DEBUG("node_it_ reach end, node_id={}", cur_node_->ID());
        return Status::C_OK;
      }

      RETURN_ERR_NOT_OK(nextLeafNode());
      if (node_it_reach_end_) {
        DEBUG("node_it_ reach end");
        return Status::C_OK;
      }

      DEBUG("find next leaf node, node_id={}", cur_node_->ID());
      resetCurKV();
    }
  }

  return Status::C_OK;
}

void BptIterator::resetCurKV() {
  cur_kv_ = cur_node_->kvs_.begin();
  kv_it_reach_end_ = false;
}

void BptIterator::nextKV() {
  cur_kv_++;
  if (cur_kv_ == cur_node_->kvs_.end()) {
    kv_it_reach_end_ = true;
  }
}

Status BptIterator::nextLeafNode() {
  using_leaf_node_.push(cur_node_);
  bool child_node_runs_out = true;

  auto f = [](const BptNode* cur_node, const IteratorOptions* it_opts) -> bool {
    return !it_opts->upper_open
               ? IsCmpGt(DataCmp(cur_node->Stat().min_key, it_opts->upper))
               : IsCmpGe(DataCmp(cur_node->Stat().min_key, it_opts->upper));
  };

  do {
    if (cur_node_->IsRoot()) {
      DEBUG("cur_node is root, node_id={}", cur_node_->ID());
      node_it_reach_end_ = true;
      cur_node_ = nullptr;
      break;
    }

    if (child_node_runs_out) {
      auto it = cur_node_->Parent()->FindChild(cur_node_);
      AMDB_ASSERT(it != cur_node_->Parent()->children_.end()) << "cur_node not found in parent's children";

      ++it;
      if (it == cur_node_->Parent()->children_.end()) {
        cur_node_ = cur_node_->Parent();
      } else {
        cur_node_ = *it;
        if (f(cur_node_, it_opts_)) {
          node_it_reach_end_ = true;
          cur_node_ = nullptr;
          break;
        }
        RETURN_ERR_NOT_OK(cur_node_->LoadNodeFromKVStorage());
        child_node_runs_out = false;
      }
    } else {
      auto children = cur_node_->children_;
      if (children.empty()) {
        DEBUG("cur_node_ has no children, node_id={}", cur_node_->ID());
        node_it_reach_end_ = true;
        cur_node_ = nullptr;
        break;
      }

      cur_node_ = children[0];

      if (f(cur_node_, it_opts_)) {
        node_it_reach_end_ = true;
        cur_node_ = nullptr;
        break;
      }
      RETURN_ERR_NOT_OK(cur_node_->LoadNodeFromKVStorage());
    }
  } while (!cur_node_->IsLeaf());

  return C_OK;
}

bool BptIterator::reachRangeBound() const {
  AMDB_ASSERT_TRUE(cur_kv_ != cur_node_->kvs_.end());
  if (it_opts_->lower_open) {
    return IsCmpGt(DataCmp(cur_kv_->first, it_opts_->upper));
  } else {
    return !IsCmpLt(DataCmp(cur_kv_->first, it_opts_->upper));
  }
}
}  // namespace storage
}  // namespace amdb