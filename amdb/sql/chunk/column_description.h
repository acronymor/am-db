#pragma once

#include <iostream>

#include "sql/expression/expr_value_type.h"

namespace amdb {
namespace chunk {
class ColumnDescriptor {
 public:
  friend class Column;

  ColumnDescriptor(expr::ExprValueType type, uint32_t col_id);

  uint32_t ID();

  expr::ExprValueType Type();
  uint8_t TypeSize() const;

  uint32_t bytes_count = 0;
  uint32_t bytes_offset = 0;

 private:
  expr::ExprValueType type_;
  uint32_t col_id_;
};
}  // namespace chunk
}  // namespace amdb
