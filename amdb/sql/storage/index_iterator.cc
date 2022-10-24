#include "sql/storage/index_iterator.h"

namespace amdb {
namespace storage {
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
}  // namespace storage
}  // namespace amdb