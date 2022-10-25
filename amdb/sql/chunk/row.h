#pragma once

#include <iostream>

#include "sql/chunk/column.h"
#include "sql/chunk/row_description.h"
#include "sql/common/arena.h"
#include "sql/common/statuscode.h"
#include "sql/expression/expr_value.h"

namespace amdb {
namespace chunk {
using Tuple = char*;

class Row {
 public:
  ~Row() = default;

  Row(Arena* arena, RowDescriptor* desc);

  Status SetColValue(uint32_t row_id, uint32_t col_id,
                     const expr::ExprValue& value);

  expr::ExprValue GetColValue(uint32_t row_id, uint32_t col_id);

 public:
  RowDescriptor* desc;

 private:
  void setTuple(uint32_t row_id, char* value);
  Tuple tupleAt(uint32_t row_id);

  Arena* arena_;
  Tuple* data_;
};
}  // namespace chunk
}  // namespace amdb