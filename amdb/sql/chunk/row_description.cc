#include "row_description.h"

#include "common/const.h"

namespace amdb {
namespace chunk {

RowDescriptor::RowDescriptor(uint32_t row_id) : row_id_(row_id) {}

uint32_t RowDescriptor::ID() { return row_id_; }

void RowDescriptor::InitAllColDesc() {
  byte_size_ = 0;
  for (uint32_t col_id = 0; col_id < cols_.size(); col_id++) {
    cols_[col_id]->bytes_count = expr::TypeSize(cols_[col_id]->Type());
    cols_[col_id]->bytes_offset = byte_size_;
    byte_size_ += cols_[col_id]->bytes_count;
    if (expr::IsVariableSize(cols_[col_id]->Type())) {
      varlen_data_ = true;
      var_cols_.push_back(cols_[col_id]);
    }
  }
}

void RowDescriptor::AddColumnDesc(chunk::ColumnDescriptor* col_desc) {
  cols_.push_back(col_desc);
}

chunk::ColumnDescriptor* RowDescriptor::GetColumnDesc(uint32_t col_id) {
  return cols_.at(col_id);
}

uint64_t RowDescriptor::byteSize() { return byte_size_; }
}  // namespace chunk
}  // namespace amdb