#include "sql/storage/table_iterator.h"

namespace amdb {
namespace storage {
Status TableIterator::GetItem(chunk::Chunk* chunk) {
  Status status = BaseIterator::Code();
  if (status != Status::C_OK && status != Status::C_BPTREE_ITERATOR_END) {
    return status;
  }

  if (data_segment_.empty()) {
    DEBUG("data_segment_ is empty");
    return Status::C_OK;
  }

  status = chunk->PullKvData(data_segment_);

  visited_kv_count_ += data_segment_.size();
  return status;
}
}  // namespace storage
}  // namespace amdb