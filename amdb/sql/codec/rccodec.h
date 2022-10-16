#include "sql/chunk/column.h"
#include "sql/chunk/row.h"
#include "sql/codec/codec.h"

/**
 * encode/decode raw and column
 */
namespace amdb {
namespace codec {
size_t EncodeColumn(const chunk::Row* row, std::string* key, std::string* value);
size_t DecodeColumn(const std::string& key, const std::string& value, chunk::Row* row);
}  // namespace codec
}  // namespace amdb
