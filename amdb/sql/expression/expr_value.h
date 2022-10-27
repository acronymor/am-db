#pragma once

#include "sql/expression/expr_value_type.h"

namespace amdb {
namespace expr {
class ExprValue {
 public:
  union {
    uint8_t uint8_value;
    uint32_t uint32_value;
    uint64_t uint64_value;
    float float32_value;
    double double_value;
  } u;

  ExprValueType Type() const;

 public:
  static ExprValue NewNull(ExprValueType type = Null);
  static ExprValue NewEmpty(ExprValueType type);
  static ExprValue NewBool(bool v);
  static ExprValue NewInt8(int8_t v);
  static ExprValue NewUInt8(uint8_t v);
  static ExprValue NewInt32(int32_t v);
  static ExprValue NewUInt32(uint32_t v);
  static ExprValue NewInt64(int64_t v);
  static ExprValue NewUInt64(uint64_t v);
  static ExprValue NewFloat(float v);
  static ExprValue NewDouble(double v);
  static ExprValue NewString(std::string&& v);

  bool BoolValue() const;
  int8_t Int8Value() const;
  uint8_t UInt8Value() const;
  int32_t Int32Value() const;
  uint32_t UInt32Value() const;
  int64_t Int64Value() const;
  uint64_t UInt64Value() const;
  float FloatValue() const;
  double DoubleValue() const;
  std::string StringValue() const;

  size_t Length();
  std::string ToString();

 private:
  explicit ExprValue(ExprValueType type, bool null = false);

  ExprValueType type_ = Invalid;

  void* var_ptr = nullptr;
  ExprValueType alloc_type = Invalid;

  void setVarPtr(ExprValueType alloc_t, void* p) {
    var_ptr = p;
    alloc_type = alloc_t;
  }
};

// save ExprValue into char*
void ToData(const expr::ExprValue& value, char* col_ptr);

// convert char* to ExprValue
void ToExprValue(const char* col_ptr, expr::ExprValue& value);

}  // namespace expr
}  // namespace amdb
