#pragma once

#include <bitset>
#include <iostream>

#include "sql/chunk/column_description.h"
#include "sql/common/arena.h"
#include "sql/expression/expr_value.h"

namespace amdb {
namespace chunk {
class Column {
 public:
  ~Column() = default;
  Column(Arena* arena, const ColumnDescriptor* col_desc, uint32_t capacity);

  int8_t GetInt8(uint32_t row_idx) const;
  bool GetBool(uint32_t row_idx) const;
  uint8_t GetUInt8(uint32_t row_idx) const;
  uint64_t GetUInt64(uint32_t row_idx) const;
  double GetDouble(uint32_t row_idx) const;
  std::string GetString(uint32_t row_idx) const;

  void SetNull(uint32_t row_idx);
  void SetBool(uint32_t row_idx, bool value);
  void SetInt8(uint32_t row_idx, int8_t value);
  void SetUInt8(uint32_t row_idx, uint8_t value);
  void SetInt32(uint32_t row_idx, int32_t value);
  void SetUInt32(uint32_t row_idx, uint32_t value);
  void SetInt64(uint32_t row_idx, int64_t value);
  void SetUInt64(uint32_t row_idx, uint64_t value);
  void SetDouble(uint32_t row_idx, double value);
  void SetString(uint32_t row_idx, const std::string& value);

  void SetExprValue(uint32_t row_idx, const expr::ExprValue& value);

 public:
  const ColumnDescriptor* desc;

 private:
  Arena* arena_;
  uint32_t capacity_{0};
  // TODO column_size = 1024, need to modify
  std::bitset<1024> null_bitmap_;
  char* data_;
};

}  // namespace chunk
}  // namespace amdb
