#include "row_description.h"

#include "common/const.h"

namespace amdb {
namespace chunk {

RowDescriptor::RowDescriptor(uint32_t row_id) : row_id_(row_id) {}

uint32_t RowDescriptor::ID() { return row_id_; }

void RowDescriptor::InitAllColDesc() {
  byte_size_ = 0;
  for (uint32_t col_id = 0; col_id < cols.size(); col_id++) {
    cols[col_id]->bytes_count = expr::TypeSize(cols[col_id]->Type());
    cols[col_id]->bytes_offset = byte_size_;
    byte_size_ += cols[col_id]->bytes_count;
    if (expr::IsVariableSize(cols[col_id]->Type())) {
      varlen_data_ = true;
      var_cols.push_back(cols[col_id]);
    }
  }
}

void RowDescriptor::AddColumnDesc(chunk::ColumnDescriptor* col_desc) {
  cols.push_back(col_desc);
}

chunk::ColumnDescriptor* RowDescriptor::GetColumnDesc(uint32_t col_id) {
  return cols.at(col_id);
}

uint64_t RowDescriptor::ByteSize() { return byte_size_; }

std::vector<ColumnDescriptor*>& RowDescriptor::GetAllColumnDesc() {
  return cols;
}
}  // namespace chunk
}  // namespace amdb