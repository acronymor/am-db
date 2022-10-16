#include "sql/chunk/row.h"

#include "common/log.h"

namespace amdb {
namespace chunk {
Tuple Row::tupleAt(uint32_t row_id) { return data_[row_id]; }

void Row::setTuple(uint32_t row_id, char* value) { data_[row_id] = value; }

Row::Row(Arena* arena, RowDescriptor* desc) : arena_(arena), desc_(desc) {
  size_t row_size = desc->cols_.size() * sizeof(Tuple);
  data_ = reinterpret_cast<Tuple*>(arena->AllocateBytes(row_size));
  memset(data_, 0, row_size);
}

Status Row::SetColValue(uint32_t row_id, uint32_t col_id,
                        const expr::ExprValue& value) {
  Tuple tuple = tupleAt(row_id);
  if (tuple == nullptr) {
    tuple = reinterpret_cast<char*>(arena_->AllocateBytes(desc_->byteSize()));
    setTuple(row_id, tuple);
  }
  if (value.Type() != desc_->GetColumnDesc(col_id)->Type()) {
    WARN("column {} type is not equal to type of schema", col_id);
    return Status::C_COLUMN_DESC_ERROR;
  }

  ColumnDescriptor* col_desc = desc_->GetColumnDesc(col_id);
  char* col_ptr = tuple + col_desc->bytes_offset;
  expr::ToData(value, col_ptr);
  return C_OK;
}

expr::ExprValue Row::GetColValue(uint32_t row_id, uint32_t col_id) {
  Tuple tuple = tupleAt(row_id);
  if (tuple == nullptr) {
    return expr::ExprValue::NewNull();
  }

  ColumnDescriptor* col_desc = desc_->GetColumnDesc(col_id);
  expr::ExprValue value = expr::ExprValue::NewEmpty(col_desc->Type());
  char* col_ptr = tuple + col_desc->bytes_offset;
  expr::ToExprValue(col_ptr, value);
  return value;
}
}  // namespace chunk
}  // namespace amdb
