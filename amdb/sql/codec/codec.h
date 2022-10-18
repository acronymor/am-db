#pragma once

#include <string>

#include "sql/expression/expr_value.h"

/**
 * encode/decode basic type
 */
namespace amdb {
namespace codec {
size_t EncodeBytes(const std::string& in, std::string* out);
size_t DecodeBytes(const std::string& in, std::string* out);

size_t EncodeUInt8(uint8_t in, std::string* out);
size_t DecodeUInt8(const std::string& in, uint8_t* out);

size_t EncodeUInt32(uint32_t in, std::string* out);
size_t DecodeUInt32(const std::string& in, uint32_t* out);

size_t EncodeExprValue(const expr::ExprValue& in, std::string* out);
size_t DecodeExprValue(const std::string& in, expr::ExprValue* out);
}  // namespace codec
}  // namespace amdb