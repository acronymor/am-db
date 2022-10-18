#pragma once

#include "sql/expression/expr_value_type.h"

namespace amdb {
namespace expr {
class ExprValue {
 public:
  union {
    int8_t int8_value;
    uint8_t uint8_value;
    uint32_t uint32_value;
    int64_t int64_value;
    uint64_t uint64_value;
    float float128_value;
    double double_value;
  } u;

  ExprValueType Type() const;

 public:
  static ExprValue NewNull(ExprValueType type = Null);
  static ExprValue NewEmpty(ExprValueType type);
  static ExprValue NewBool(bool v);
  static ExprValue NewInt8(int8_t v);
  static ExprValue NewUInt8(uint8_t v);
  static ExprValue NewUInt32(uint32_t v);
  static ExprValue NewInt64(int64_t v);
  static ExprValue NewUInt64(uint64_t v);
  static ExprValue NewFloat(float v);
  static ExprValue NewDouble(double v);

  size_t Length();

 private:
  ExprValueType type_ = Invalid;
  explicit ExprValue(ExprValueType type, bool null = false);
};

// save ExprValue into char*
void ToData(const expr::ExprValue& value, char* col_ptr);

// convert char* to ExprValue
void ToExprValue(const char* col_ptr, expr::ExprValue& value);

}  // namespace expr
}  // namespace amdb
