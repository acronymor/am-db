#include "sql/expression/expr_value.h"

#include "common/assert.h"
#include "common/log.h"

namespace amdb {
namespace expr {

void ToData(const expr::ExprValue& value, char* col_ptr, Arena* arena) {
  switch (value.Type()) {
    case expr::Int8:
      *reinterpret_cast<int8_t*>(col_ptr) = value.Int8Value();
      break;
    case expr::UInt8:
      *reinterpret_cast<uint8_t*>(col_ptr) = value.UInt8Value();
      break;
    case expr::Int32:
      *reinterpret_cast<int32_t*>(col_ptr) = value.Int32Value();
      break;
    case expr::UInt32:
      *reinterpret_cast<uint32_t*>(col_ptr) = value.UInt32Value();
      break;
    case expr::Int64:
      *reinterpret_cast<int64_t*>(col_ptr) = value.Int64Value();
      break;
    case expr::UInt64:
      *reinterpret_cast<uint64_t*>(col_ptr) = value.UInt64Value();
      break;
    case expr::String: {
      AMDB_ASSERT_FALSE(arena == nullptr);
      std::string str = value.StringValue();
      char* str_copy = reinterpret_cast<char*>(arena->AllocateBytes(str.size()));
      memcpy(str_copy, str.data(), str.length());
      *reinterpret_cast<uint64_t*>(col_ptr) = reinterpret_cast<uint64_t>(str_copy);
      *reinterpret_cast<uint32_t*>(col_ptr + sizeof(uint64_t)) = str.length();
    } break;
    default:
      ERROR("Not support type {}", value.Type());
      break;
  }
}

void ToExprValue(const char* col_ptr, expr::ExprValue& value) {
  char* ptr = const_cast<char*>(col_ptr);
  switch (value.Type()) {
    case expr::Int8:
      value.u.uint8_value = *(reinterpret_cast<int8_t*>(ptr));
      break;
    case expr::UInt8:
      value.u.uint8_value = *(reinterpret_cast<uint8_t*>(ptr));
      break;
    case expr::Int32:
      value.u.uint32_value = *(reinterpret_cast<int32_t*>(ptr));
      break;
    case expr::UInt32:
      value.u.uint32_value = *(reinterpret_cast<uint32_t*>(ptr));
      break;
    case expr::Int64:
      value.u.uint64_value = *(reinterpret_cast<int64_t*>(ptr));
      break;
    case expr::UInt64:
      value.u.uint64_value = *(reinterpret_cast<uint64_t*>(ptr));
      break;
    case expr::String: {
      uint64_t* str_ptr = reinterpret_cast<uint64_t*>(ptr);
      uint32_t* str_size = reinterpret_cast<uint32_t*>(ptr + sizeof(uint64_t));
      std::string* str = new std::string();
      str->resize(*str_size);
      str->assign(reinterpret_cast<char*>(*str_ptr), *str_size);

      value.SetVarPtr(ExprValueType::String, str);
    } break;
    default:
      ERROR("Not support type {}", value.Type());
      break;
  }
}

ExprValue::~ExprValue() {
  // delete var_ptr, var_ptr = nullptr;
}

ExprValue::ExprValue(ExprValueType type, bool null) { type_ = type; }

ExprValueType ExprValue::Type() const { return type_; }

ExprValue ExprValue::NewNull(ExprValueType type) { return ExprValue(type, true); }

ExprValue ExprValue::NewEmpty(ExprValueType type) { return ExprValue(type, false); }

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

ExprValue ExprValue::NewFloat32(float v) {
  ExprValue value(ExprValueType::Float32, false);
  value.u.float32_value = v;
  return value;
}

ExprValue ExprValue::NewFloat64(double v) {
  ExprValue value(ExprValueType::Float64, false);
  value.u.double_value = v;
  return value;
}

ExprValue ExprValue::NewFloat128(long double v) {
  ExprValue value(ExprValueType::Float128, false);
  value.u.float128_value = v;
  return value;
}

ExprValue ExprValue::NewDouble(double v) {
  ExprValue value(ExprValueType::Double, false);
  value.u.double_value = v;
  return value;
}

ExprValue ExprValue::NewString(std::string&& v, Arena* arena) {
  AMDB_ASSERT_FALSE(arena == nullptr);
  ExprValue value(ExprValueType::String, false);
  value.SetVarPtr(ExprValueType::String, new std::string(std::move(v)));
  return value;
}

ExprValue ExprValue::NewMax(ExprValueType type) {
  ExprValue value = ExprValue::NewEmpty(type);
  switch (type) {
    case Bool:
    case UInt8:
      value = ExprValue::NewUInt8(std::numeric_limits<uint8_t>::max());
      break;
    case Int8:
      value = ExprValue::NewInt8(std::numeric_limits<int8_t>::max());
      break;
    case UInt32:
      value = ExprValue::NewUInt32(std::numeric_limits<uint32_t>::max());
      break;
    case Int32:
      value = ExprValue::NewInt32(std::numeric_limits<int32_t>::max());
      break;
    case UInt64:
      value = ExprValue::NewUInt64(std::numeric_limits<uint64_t>::max());
      break;
    case Int64:
      value = ExprValue::NewInt64(std::numeric_limits<int64_t>::max());
      break;
    case Float32:
      value = ExprValue::NewFloat32(std::numeric_limits<float>::max());
      break;
    case Float64:
      value = ExprValue::NewFloat64(std::numeric_limits<double>::max());
      break;
    case Float128:
      value = ExprValue::NewFloat128(std::numeric_limits<long double>::max());
      break;
    case String:
      value.SetVarPtr(type, new std::string());
      break;
  };

  return value;
}

ExprValue ExprValue::NewMin(ExprValueType type) {
  ExprValue value = ExprValue::NewEmpty(type);
  switch (type) {
    case Bool:
    case UInt8:
      value = ExprValue::NewUInt8(std::numeric_limits<uint8_t>::min());
      break;
    case Int8:
      value = ExprValue::NewInt8(std::numeric_limits<int8_t>::min());
      break;
    case UInt32:
      value = ExprValue::NewUInt32(std::numeric_limits<uint32_t>::min());
      break;
    case Int32:
      value = ExprValue::NewInt32(std::numeric_limits<int32_t>::min());
      break;
    case UInt64:
      value = ExprValue::NewUInt64(std::numeric_limits<uint64_t>::min());
      break;
    case Int64:
      // value = ExprValue::NewInt64(std::numeric_limits<int64_t>::min());
      value.u.uint64_value = 0;
      break;
    case Float32:
      value.u.float32_value = 0.0f;
      break;
    case Float64:
      value.u.double_value = 0.0;
      break;
    case Float128:
      value.u.float128_value = 0.0;
      break;
    case String:
      value.SetVarPtr(type, new std::string());
      break;
  };

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
  // return std::string(static_cast<char*>(var_ptr));
  return *(static_cast<std::string*>(var_ptr));
}

size_t ExprValue::Length() {
  // basic type
  return TypeSize(type_);
}

std::string ExprValue::ToString() {
  std::string value;
  switch (type_) {
    case expr::Int8:
      value = std::to_string(Int8Value());
      break;
    case expr::UInt8:
      value = std::to_string(UInt8Value());
      break;
    case expr::Int32:
      value = std::to_string(Int32Value());
      break;
    case expr::UInt32:
      value = std::to_string(UInt32Value());
      break;
    case expr::Int64:
      value = std::to_string(Int64Value());
      break;
    case expr::UInt64:
      value = std::to_string(UInt64Value());
      break;
    case expr::Float32:
      value = std::to_string(FloatValue());
      break;
    case expr::Double:
      value = std::to_string(DoubleValue());
      break;
    case expr::String:
      value = StringValue();
      break;
    default:
      break;
  }
  return value;
}

void ExprValue::SetVarPtr(ExprValueType alloc_t, void* p) {
  var_ptr = p;
  alloc_type = alloc_t;
}
}  // namespace expr
}  // namespace amdb