#include "sql/storage/iterator.h"

namespace amdb {
namespace storage {

BaseIterator::BaseIterator(Index* index, uint64_t page_size)
    : index_(index), page_size_(page_size) {}

Status BaseIterator::Open() {
  while (current_op_idx_ < it_ops_.size()) {
    IteratorOptions& it_op = it_ops_.at(current_op_idx_++);
    tree_it_ = index_->TreeCtxx()->AllocMem()->CreateObject<BptIterator>(
        index_->Tree(), &it_op);
    Status status = tree_it_->Open();
    if (Status::C_OK != status) {
      if (Status::C_BPTREE_ITERATOR_END == status) {
        DEBUG("Empty Scan, tree node_id={}", index_->Tree()->Root()->ID());
        continue;
      }

      ERROR("Iterator init failed, node_id={}", index_->Tree()->Root()->ID());
      return status;
    }
    return Status::C_OK;
  }

  code_ = Status::C_BPTREE_ITERATOR_END;
  return Status::C_OK;
}

void BaseIterator::Next() {
  if (!Valid()) {
    return;
  }

  data_segment_.clear();

  while (code_ == Status::C_OK && data_segment_.size() < page_size_) {
    if (tree_it_->HashNext()) {
      tree_it_->Next(&data_segment_, page_size_ - data_segment_.size());
    } else {
      code_ = Open();
    };
  }
}

Status BaseIterator::InitIterOptions(const IteratorOptions& it_opt) {
  it_ops_.push_back(it_opt);

  return Status::C_OK;
}

}  // namespace storage
}  // namespace amdb