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

Status IndexIterator::GetItem(chunk::Chunk* chunk) {
  Status code = BaseIterator::Code();
  if (code != Status::C_OK && code != Status::C_BPTREE_ITERATOR_END) {
    return code;
  }

  if (data_segment_.empty()) {
    DEBUG("data_segment_ is empty");
    return Status::C_OK;
  }

  Status status = chunk->PullIndexData(data_segment_);

  visited_kv_count_ += data_segment_.size();
  return status;
}

Status IndexIterator::InitIterOptions(const IteratorOptions& it_opt) {
  it_ops_.push_back(it_opt);

  return Status::C_OK;
}

Status TableIterator::GetItem(chunk::Chunk* chunk) {
  Status status = BaseIterator::Code();
  if (status != Status::C_OK && status != Status::C_BPTREE_ITERATOR_END) {
    return status;
  }

  if (data_segment_.empty()) {
    DEBUG("data_segment_ is empty");
    return Status::C_OK;
  }

  auto f = [](const std::string& key) -> std::string&& {
    std::string current_key = key;
    return std::move(current_key);
  };

  std::vector<std::pair<std::string, std::string>> data_segment;
  RETURN_ERR_NOT_OK(kv_util->GetRecords(data_segment_, &data_segment, f));

  status = chunk->PullKvData(data_segment);

  visited_kv_count_ += data_segment_.size();
  return status;
}

}  // namespace storage
}  // namespace amdb