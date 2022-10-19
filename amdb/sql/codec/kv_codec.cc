#include "sql/codec/kv_codec.h"

namespace amdb {
namespace codec {

size_t EncodeTreeNodeKey(uint64_t db_id, uint64_t table_id, uint64_t index_id,
                         uint64_t node_id, std::string* output) {
  size_t offset = 0;
  offset += EncodeUInt64(db_id, output);
  offset += EncodeUInt64(table_id, output);
  offset += EncodeUInt64(index_id, output);
  offset += EncodeUInt64(node_id, output);
  return offset;
}

size_t EncodeTableMetaKey(uint64_t db_id, uint64_t table_id,
                          std::string* output) {
  size_t offset = 0;
  offset += EncodeUInt64(db_id, output);
  offset += EncodeUInt64(table_id, output);
  return offset;
}
}  // namespace codec
}  // namespace amdb