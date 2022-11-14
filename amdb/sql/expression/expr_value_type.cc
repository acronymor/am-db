#include "sql/expression/expr_value_type.h"

#include "common/log.h"

namespace amdb {
namespace expr {
uint8_t TypeSize(const ExprValueType& type) {
  uint8_t size = 255;
  switch (type) {
    case expr::Int8:
    case expr::UInt8:
      size = 1;
      break;
    case expr::Int32:
    case expr::UInt32:
      size = 4;
      break;
    case expr::Int64:
    case expr::UInt64:
      size = 8;
      break;
    case expr::String:
      // str_addr[8] + str_len[4]
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
