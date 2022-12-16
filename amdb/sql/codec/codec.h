#pragma once

#include <string>

#include "sql/expression/expr_value.h"

/**
 * encode/decode basic type
 * // TODO may be introduce into multi copy both string, need to optimization
 */
namespace amdb {
namespace codec {
size_t EncodeBytes(const std::string& in, std::string* out);
size_t DecodeBytes(const std::string& in, std::string* out);

size_t EncodeUInt8(uint8_t in, std::string* out);
size_t DecodeUInt8(const std::string& in, uint8_t* out);

size_t EncodeUInt32(uint32_t in, std::string* out);
size_t DecodeUInt32(const std::string& in, uint32_t* out);

size_t EncodeInt32(int32_t in, std::string* out);
size_t DecodeInt32(const std::string& in, int32_t* out);

size_t EncodeUInt64(uint64_t in, std::string* out);
size_t DecodeUInt64(const std::string& in, uint64_t* out);

size_t EncodeInt64(int64_t in, std::string* out);
size_t DecodeInt64(const std::string& in, int64_t* out);

size_t EncodeExprValue(const expr::ExprValue& in, std::string* out);
size_t DecodeExprValue(const std::string& in, expr::ExprValue* out, Arena* arena = nullptr);
}  // namespace codec
}  // namespace amdb