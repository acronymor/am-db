#include "sql/storage/iterator.h"

namespace amdb {
namespace storage {

BaseIterator::BaseIterator(Bptree* bptree, TreeCtx* tree_ctx,
                           uint64_t page_size) {
  bptree_ = bptree;
  tree_ctx_ = tree_ctx;
  page_size_ = page_size;
}

Status BaseIterator::Open() {
  while (current_op_idx_ < it_ops_.size()) {
    IteratorOptions& it_op = it_ops_.at(current_op_idx_++);
    tree_it_ =
        tree_ctx_->AllocMem()->CreateObject<BptIterator>(bptree_, &it_op);
    Status status = tree_it_->Open();
    if (Status::C_OK != status) {
      return status;
    }
  }

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

}  // namespace storage
}  // namespace amdb