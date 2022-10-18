#include "sql/chunk/column.h"
#include "sql/chunk/row.h"
#include "sql/codec/codec.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace codec {
/**
 * encode/decode index
 */
size_t EncodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, chunk::Row* row,
                   std::string* key, std::string* value);

size_t DecodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, std::string* key,
                   std::string* value, chunk::Row* row);

size_t EncodeRow(const schema::TableInfo* table_info, chunk::Row* row,
                 std::string* key, std::string* value);
}  // namespace codec
}  // namespace amdb