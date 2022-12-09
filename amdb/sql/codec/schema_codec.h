#pragma once

#include "sql/codec/codec.h"

namespace amdb {
namespace codec {
const uint64_t kGlobalDatabaseID = 0;
const uint64_t kGlobalTableID = 0;

enum MetaType : uint8_t {
  kDatabaseId = 0,
  kDatabaseInfo = 1,
  kTableId = 2,
  kTableInfo = 3
};

enum struct DataType : uint8_t {
  kTreeNode = 0,
  kRow = 1,
  kDatabaseId = 2,
  kTableId = 3,
  kColumnId = 4,
};

size_t EncodeDatabaseAllocID(uint64_t id, std::string* out);
size_t EncodeTableAllocID(uint64_t id, std::string* out);
size_t EncodeColumnAllocID(uint64_t id, std::string* out);

size_t EncodeDatabaseID(uint64_t db_id, std::string* out);
size_t DecodeDatabaseID(const std::string& in, uint64_t* db_id);

size_t EncodeDatabaseName(const std::string& db_name, std::string* out);
size_t DecodeDatabaseName(const std::string& in, std::string* db_name);

size_t EncodeTableID(const uint64_t db_id, const uint64_t table_id, std::string* out);
size_t DecodeTableID(const std::string in, uint64_t* db_id, uint64_t* table_id);

size_t EncodeTableName(const uint64_t db_id, const std::string& table_name, std::string* out);
size_t DecodeTableName(const uint64_t in, uint64_t* db_id, std::string* table_name);

size_t EncodeDataKey(const uint64_t db_id, const uint64_t table_id, const std::string& key, std::string* out);

size_t EncodeIndexTreeNodeID(const uint64_t db_id, const uint64_t table_id, const uint64_t index_id,
                             const uint64_t node_id, std::string* out);
}  // namespace codec
}  // namespace amdb