#include "sql/codec/schema_codec.h"

namespace amdb {
namespace codec {
size_t EncodeDatabaseID(uint64_t db_id, std::string *out) {
  size_t offset = 0;
  offset += EncodeUInt64(kGlobalDatabaseID, out);
  offset += EncodeUInt64(kGlobalTableID, out);
  offset += EncodeUInt8(MetaType::kDatabaseId, out);
  offset += EncodeUInt64(db_id, out);
  return offset;
}

size_t DecodeDatabaseID(const std::string &in, uint64_t *db_id) {
  size_t offset = sizeof(uint64_t) + sizeof(uint64_t) + sizeof(uint8_t);

  std::string res;
  res.assign(in.data() + offset, in.length() - offset);
  offset += DecodeUInt64(res, db_id);

  return offset;
}

size_t EncodeDatabaseName(const std::string &db_name, std::string *out) {
  size_t offset = 0;
  offset += EncodeUInt64(kGlobalDatabaseID, out);
  offset += EncodeUInt64(kGlobalTableID, out);
  offset += EncodeUInt8(MetaType::kDatabaseId, out);
  offset += EncodeBytes(db_name, out);
  return offset;
}

size_t DecodeDatabaseName(const std::string &in, std::string *out) {
  return 0;
}

size_t EncodeTableID(const uint64_t db_id, const uint64_t table_id,
                     std::string *out) {
  size_t offset = 0;
  offset += EncodeUInt64(kGlobalDatabaseID, out);
  offset += EncodeUInt64(kGlobalTableID, out);
  offset += EncodeUInt8(MetaType::kTableId, out);
  offset += EncodeUInt64(db_id, out);
  offset += EncodeUInt8(table_id, out);
  return offset;
}

size_t DecodeTableID(const std::string in, uint64_t *db_id,
                     uint64_t *table_id) {
  return 0;
}

size_t EncodeTableName(const uint64_t db_id, const std::string &table_name,
                       std::string *out) {
  size_t offset = 0;
  offset += EncodeUInt64(kGlobalDatabaseID, out);
  offset += EncodeUInt64(kGlobalTableID, out);
  offset += EncodeUInt8(MetaType::kTableInfo, out);
  offset += EncodeUInt64(db_id, out);
  offset += EncodeBytes(table_name, out);
  return offset;
}

size_t DecodeTableName(const uint64_t in, uint64_t *db_id,
                       std::string *table_name) {
  return 0;
}

size_t EncodeIndexTreeNodeID(const uint64_t db_id, const uint64_t table_id,
                             const uint64_t index_id, const uint64_t node_id,
                             std::string *out) {
  size_t offset = 0;
  offset += EncodeUInt64(db_id, out);
  offset += EncodeUInt64(table_id, out);
  offset += EncodeUInt8(DataType::kTreeNode, out);
  offset += EncodeUInt64(index_id, out);
  offset += EncodeUInt64(node_id, out);
  return offset;
}

size_t EncodeDataKey(const uint64_t db_id, const uint64_t table_id,
                     const std::string &key, std::string *out) {
  size_t offset = 0;
  offset += EncodeUInt64(db_id, out);
  offset += EncodeUInt64(table_id, out);
  offset += EncodeUInt8(DataType::kRow, out);
  offset += EncodeBytes(key, out);
  return offset;
}

}  // namespace codec
}  // namespace amdb
