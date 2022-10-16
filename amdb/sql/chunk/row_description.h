#pragma once

#include <iostream>
#include <vector>

#include "sql/chunk/column_description.h"

namespace amdb {
namespace chunk {
class RowDescriptor {
 public:
  friend class Row;

  explicit RowDescriptor(uint32_t row_id);

  uint32_t ID();

  void InitAllColDesc();

  void AddColumnDesc(chunk::ColumnDescriptor* col_desc);

  chunk::ColumnDescriptor* GetColumnDesc(uint32_t col_id_);

 private:
  std::vector<ColumnDescriptor*> cols_;
  std::vector<ColumnDescriptor*> var_cols_;

  uint64_t byteSize();
  uint64_t byte_size_ = 0;
  bool varlen_data_ = false;

  uint32_t row_id_;
};
}  // namespace chunk
}  // namespace amdb
