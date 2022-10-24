#include "expr_value_type.h"

#include "common/log.h"

namespace amdb {
namespace expr {
uint8_t TypeSize(const ExprValueType& type) {
  uint8_t size = 255;
  switch (type) {
    case Int8:
    case UInt8:
      size = 1;
      break;
    case Int32:
    case UInt32:
      size = 4;
      break;
    case Int64:
    case UInt64:
      size = 8;
      break;
    case String:
      size = 12;
      break;
    default:
      ERROR("Not support type {}", type);
      break;
  }

  return size;
}

bool IsVariableSize(ExprValueType type) {
  return type == ExprValueType::String;
}
}  // namespace expr
}  // namespace amdb
