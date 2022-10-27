#include "sql/expression/expr_value.h"

#include "common/assert.h"
#include "common/log.h"

namespace amdb {
namespace expr {

void ToData(const expr::ExprValue& value, char* col_ptr) {
  switch (value.Type()) {
    case expr::Int8:
      *reinterpret_cast<int8_t*>(col_ptr) = value.u.uint8_value;
      break;
    case expr::UInt8:
      *reinterpret_cast<uint8_t*>(col_ptr) = value.u.uint8_value;
      break;
    case expr::UInt32:
      *reinterpret_cast<uint32_t*>(col_ptr) = value.u.uint32_value;
      break;
    case expr::Int64:
      *reinterpret_cast<int64_t*>(col_ptr) = value.u.uint64_value;
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
  value.u.uint8_value = v;
  return value;
}

ExprValue ExprValue::NewUInt8(uint8_t v) {
  ExprValue value(ExprValueType::UInt8, false);
  value.u.uint8_value = v;
  return value;
}

ExprValue ExprValue::NewInt32(int32_t v) {
  ExprValue value(ExprValueType::Int32, false);
  value.u.uint32_value = v;
  return value;
}

ExprValue ExprValue::NewUInt32(uint32_t v) {
  ExprValue value(ExprValueType::UInt32, false);
  value.u.uint32_value = v;
  return value;
}

ExprValue ExprValue::NewInt64(int64_t v) {
  ExprValue value(ExprValueType::Int64, false);
  value.u.uint64_value = v;
  return value;
}

ExprValue ExprValue::NewUInt64(uint64_t v) {
  ExprValue value(ExprValueType::UInt64, false);
  value.u.uint64_value = v;
  return value;
}

ExprValue ExprValue::NewFloat(float v) {
  ExprValue value(ExprValueType::Float128, false);
  value.u.float32_value = v;
  return value;
}

ExprValue ExprValue::NewDouble(double v) {
  ExprValue value(ExprValueType::Double, false);
  value.u.double_value = v;
  return value;
}

ExprValue ExprValue::NewString(std::string&& v) {
  ExprValue value(ExprValueType::String, false);
  value.type_ = ExprValueType::String;
  value.setVarPtr(ExprValueType::String, new std::string(v));
  return value;
}

bool ExprValue::BoolValue() const {
  AMDB_ASSERT_EQ(ExprValueType::Bool, type_);
  return u.uint8_value != 0;
}

int8_t ExprValue::Int8Value() const {
  AMDB_ASSERT_EQ(ExprValueType::Int8, type_);
  return u.uint8_value;
}

uint8_t ExprValue::UInt8Value() const {
  AMDB_ASSERT_EQ(ExprValueType::UInt8, type_);
  return u.uint8_value;
}

int32_t ExprValue::Int32Value() const {
  AMDB_ASSERT_EQ(ExprValueType::Int32, type_);
  return u.uint32_value;
}

uint32_t ExprValue::UInt32Value() const {
  AMDB_ASSERT_EQ(ExprValueType::UInt32, type_);
  return u.uint32_value;
}

int64_t ExprValue::Int64Value() const {
  AMDB_ASSERT_EQ(ExprValueType::Int64, type_);
  return u.uint64_value;
}

uint64_t ExprValue::UInt64Value() const {
  AMDB_ASSERT_EQ(ExprValueType::UInt64, type_);
  return u.uint64_value;
}

float ExprValue::FloatValue() const {
  AMDB_ASSERT_EQ(ExprValueType::Float32, type_);
  return u.float32_value;
}

double ExprValue::DoubleValue() const {
  AMDB_ASSERT_EQ(ExprValueType::Double, type_);
  return u.double_value;
}

std::string ExprValue::StringValue() const {
  AMDB_ASSERT_EQ(ExprValueType::String, type_);
  return *(static_cast<std::string*>(var_ptr));
}

size_t ExprValue::Length() {
  // basic type
  return sizeof(type_);
}

std::string ExprValue::ToString() {
  std::string value;
  switch (type_) {
    case Int8:
      value = std::to_string(Int8Value());
      break;
    case UInt8:
      value = std::to_string(UInt8Value());
      break;
    case Int32:
      value = std::to_string(Int32Value());
      break;
    case UInt32:
      value = std::to_string(UInt32Value());
      break;
    case Int64:
      value = std::to_string(Int64Value());
      break;
    case UInt64:
      value = std::to_string(UInt64Value());
      break;
    case Float32:
      value = std::to_string(FloatValue());
      break;
    case Double:
      value = std::to_string(DoubleValue());
      break;
    case String:
      value = StringValue();
      break;
    default:
      break;
  }
  return value;
}
}  // namespace expr
}  // namespace amdb