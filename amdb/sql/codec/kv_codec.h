#pragma once

#include "sql/codec/codec.h"

/**
 * encode/decode key for storage api
 */
namespace amdb {
namespace codec {
size_t EncodeTreeNodeKey(uint64_t db_id, uint64_t table_id, uint64_t index_id,
                         uint64_t node_id, std::string* output);

size_t EncodeTableMetaKey(uint64_t db_id, uint64_t table_id, std::string* output);
}  // namespace codec
}  // namespace amdb
