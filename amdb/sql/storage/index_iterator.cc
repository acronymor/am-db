#include "sql/storage/index_iterator.h"

#include "sql/codec/rc_codec.h"
#include "sql/codec/schema_codec.h"

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

  // TODO data_segment -> keys and value
  std::vector<std::string> keys;
  std::vector<std::string> values;
  Status status = chunk->PullIndexData(index_->table_info_, index_->index_info_,
                                       keys, values);
  RETURN_ERR_NOT_OK(status);

  visited_kv_count_ += data_segment_.size();
  return Status::C_OK;
}
}  // namespace storage
}  // namespace amdb