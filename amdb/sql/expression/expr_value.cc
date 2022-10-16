#include "sql/expression/expr_value.h"
#include "common/log.h"

namespace amdb {
namespace expr {

void ToData(const expr::ExprValue& value, char* col_ptr) {
  switch (value.Type()) {
    case expr::Int8:
      *reinterpret_cast<int8_t*>(col_ptr) = value.u.int8_value;
      break;
    case expr::UInt8:
      *reinterpret_cast<uint8_t*>(col_ptr) = value.u.uint8_value;
      break;
    case expr::UInt32:
      *reinterpret_cast<uint32_t*>(col_ptr) = value.u.uint32_value;
      break;
    case expr::Int64:
      *reinterpret_cast<int64_t*>(col_ptr) = value.u.int64_value;
      break;
    case expr::UInt64:
      *reinterpret_cast<uint64_t*>(col_ptr) = value.u.uint64_value;
      break;
    default:
      ERROR("Not support type {}", value.Type());
      break;
  }
}

void ToExprValue(const char* col_ptr, expr::ExprValue& value) {
  char* ptr = const_cast<char*>(col_ptr);
  switch (value.Type()) {
    case expr::Int8:
      value = ExprValue::NewInt8(*(reinterpret_cast<int8_t*>(ptr)));
      break;
    case expr::UInt8:
      value = ExprValue::NewUInt8(*(reinterpret_cast<uint8_t*>(ptr)));
      break;
    case expr::UInt32:
      value = ExprValue::NewUInt32(*(reinterpret_cast<uint32_t*>(ptr)));
      break;
    case expr::Int64:
      value = ExprValue::NewInt64(*(reinterpret_cast<int64_t*>(ptr)));
      break;
    case expr::UInt64:
      value = ExprValue::NewUInt64(*(reinterpret_cast<uint64_t*>(ptr)));
      break;
    default:
      ERROR("Not support type {}", value.Type());
      break;
  }
}

ExprValue::ExprValue(ExprValueType type, bool null) { type_ = type; }

ExprValueType ExprValue::Type() const { return type_; }

ExprValue ExprValue::NewNull(ExprValueType type) {
  return ExprValue(type, true);
}

ExprValue ExprValue::NewEmpty(ExprValueType type) {
  ExprValue value(type, false);
  return value;
}

ExprValue ExprValue::NewBool(bool v) {
  ExprValue value(ExprValueType::Bool, false);
  value.u.uint8_value = v ? 1 : 0;
  return value;
}

ExprValue ExprValue::NewInt8(int8_t v) {
  ExprValue value(ExprValueType::Int8, false);
  value.u.int8_value = v;
  return value;
}

ExprValue ExprValue::NewUInt8(uint8_t v) {
  ExprValue value(ExprValueType::UInt8, false);
  value.u.uint8_value = v;
  return value;
}

ExprValue ExprValue::NewUInt32(uint32_t v) {
  ExprValue value(ExprValueType::UInt32, false);
  value.u.uint32_value = v;
  return value;
}

ExprValue ExprValue::NewInt64(int64_t v) {
  ExprValue value(ExprValueType::Int64, false);
  value.u.int64_value = v;
  return value;
}

ExprValue ExprValue::NewUInt64(uint64_t v) {
  ExprValue value(ExprValueType::UInt64, false);
  value.u.uint64_value = v;
  return value;
}

ExprValue ExprValue::NewFloat(float v) {
  ExprValue value(ExprValueType::Float128, false);
  value.u.float128_value = v;
  return value;
}

ExprValue ExprValue::NewDouble(double v) {
  ExprValue value(ExprValueType::Double, false);
  value.u.double_value = v;
  return value;
}
}  // namespace expr
}  // namespace amdb