#pragma once

#include <iostream>

namespace amdb {
namespace expr {
enum ExprValueType : uint8_t {
  Null = 0,
  Bool = 1,

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

  Invalid = 255
};

uint8_t TypeSize(const ExprValueType& type);

bool IsVariableSize(ExprValueType type);
}  // namespace expr
}  // namespace amdb