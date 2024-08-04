#pragma once

#include <cstdint>
#include <iostream>

namespace amdb {
namespace schema {
struct ColumnType;
}

namespace expr {
enum ExprValueType : uint8_t {
  Null = 0,
  MinInf = 2,

  Bool = 10,
  Int8 = 11,
  UInt8 = 12,
  Int32 = 15,
  UInt32 = 16,
  Int64 = 17,
  UInt64 = 18,

  Float32 = 21,
  Float64 = 22,
  Float128 = 23,
  Double = 25,

  String = 40,

  MaxInf = 250,
  Invalid = 255
};

uint8_t TypeSize(const ExprValueType& type);

bool IsVariableSize(ExprValueType type);

ExprValueType ToExprType(const schema::ColumnType& col_type);
}  // namespace expr
}  // namespace amdb