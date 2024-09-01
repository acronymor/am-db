#include "sql/codec/statistic_codec.h"

#include "sql/codec/codec.h"

namespace amdb {
namespace codec {
size_t EncodeMetaStatistic(const std::uint64_t db_id, const std::uint64_t table_id, std::string* out) {
  size_t offset = 0;
  offset += EncodeUInt64(db_id, out);
  offset += EncodeUInt64(table_id, out);
  offset += EncodeUInt8(static_cast<uint8_t>(MetaType::kMetaStatistic), out);
  return offset;
}

size_t EncodeColumnStatistic(const std::uint64_t db_id, const std::uint64_t table_id, const std::uint64_t column_id,
                             std::string* out) {
  size_t offset = 0;
  offset += EncodeUInt64(db_id, out);
  offset += EncodeUInt64(table_id, out);
  offset += EncodeUInt64(column_id, out);
  offset += EncodeUInt8(static_cast<uint8_t>(MetaType::kColumnStatistic), out);
  return offset;
}
}  // namespace codec
}  // namespace amdb
