#pragma once

#include "sql/chunk/column.h"
#include "sql/chunk/row.h"
#include "sql/schema/schema.h"

/**
 * encode/decode row/column/index of table
 */
namespace amdb {
namespace codec {
size_t EncodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, chunk::Row* row,
                   std::string* key, std::string* value);

size_t DecodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, std::string* key,
                   std::string* value, chunk::Row* row);

size_t EncodeRow(const schema::TableInfo* table_info, chunk::Row* row,
                 std::string* key, std::string* value);

size_t DecodeRow(Arena* arena, const schema::TableInfo* table_info,
                 std::string* value, chunk::Row* row);
}  // namespace codec
}  // namespace amdb