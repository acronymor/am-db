#include "sql/chunk/column.h"
#include "common/log.h"

namespace amdb {
namespace chunk {
Column::Column(Arena *arena, const ColumnDescriptor *col_desc,
               uint32_t capacity)
    : arena_(arena), desc(col_desc), capacity_(capacity) {
  data_ = reinterpret_cast<char *>(
      arena_->AllocateBytes(capacity_ * desc->TypeSize()));
}

void Column::SetNull(uint32_t row_idx) { null_bitmap_[row_idx] = true; }

void Column::SetBool(uint32_t row_idx, bool value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<uint8_t *>(data_ptr) = value;
}

void Column::SetInt8(uint32_t row_idx, int8_t value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<int8_t *>(data_ptr) = value;
}

void Column::SetUInt8(uint32_t row_idx, uint8_t value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<uint8_t *>(data_ptr) = value;
}

void Column::SetInt64(uint32_t row_idx, int64_t value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<int64_t *>(data_ptr) = value;
}

void Column::SetUInt32(uint32_t row_idx, uint32_t value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<uint32_t *>(data_ptr) = value;
}

void Column::SetInt32(uint32_t row_idx, int32_t value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<int32_t *>(data_ptr) = value;
}

void Column::SetUInt64(uint32_t row_idx, uint64_t value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<uint64_t *>(data_ptr) = value;
}

void Column::SetDouble(uint32_t row_idx, double value) {
  null_bitmap_[row_idx] = false;
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<double *>(data_ptr) = value;
}

void Column::SetString(uint32_t row_idx, const std::string &value) {
  null_bitmap_[row_idx] = false;
  uint32_t len = value.length();

  char *data = reinterpret_cast<char *>(arena_->AllocateBytes(value.size()));
  memcpy(data, value.data(), len);

  char *data_ptr = data_ + row_idx * desc->TypeSize();
  *reinterpret_cast<uint32_t *>(data_ptr) = len;
  *reinterpret_cast<char *>(data_ptr + len) = *data;
}

int8_t Column::GetInt8(uint32_t row_idx) const {
  return *reinterpret_cast<int8_t *>(data_ + row_idx * desc->TypeSize());
}

uint8_t Column::GetUInt8(uint32_t row_idx) const {
  return *reinterpret_cast<uint8_t *>(data_ + row_idx * desc->TypeSize());
}

bool Column::GetBool(uint32_t row_idx) const {
  return *reinterpret_cast<uint8_t *>(data_ + row_idx * desc->TypeSize());
}

uint64_t Column::GetUInt64(uint32_t row_idx) const {
  return *reinterpret_cast<uint64_t *>(data_ + row_idx * desc->TypeSize());
}

double Column::GetDouble(uint32_t row_idx) const {
  return *reinterpret_cast<double *>(data_ + row_idx * desc->TypeSize());
}

std::string Column::GetString(uint32_t row_idx) const {
  char *data_ptr = data_ + row_idx * desc->TypeSize();
  uint32_t len = *reinterpret_cast<uint32_t *>(data_ptr);
  char *value_ptr = reinterpret_cast<char *>(data_ptr + sizeof(len));
  std::string value;
  value.assign(value_ptr, len);

  return std::move(value);
}

void Column::SetExprValue(uint32_t row_idx, const expr::ExprValue &value) {
  switch (value.Type()) {
    case expr::Int8:
      SetInt8(row_idx, value.Int8Value());
      break;
    case expr::UInt8:
      SetUInt8(row_idx, value.UInt8Value());
      break;
    case expr::Int32:
      SetInt32(row_idx, value.Int32Value());
      break;
    case expr::UInt32:
      SetUInt32(row_idx, value.UInt32Value());
      break;
    case expr::Int64:
      SetInt64(row_idx, value.Int64Value());
      break;
    case expr::UInt64:
      SetUInt64(row_idx, value.UInt64Value());
      break;
    case expr::Double:
      SetDouble(row_idx, value.DoubleValue());
      break;
    case expr::String:
      SetString(row_idx, value.StringValue());
      break;
    default:
      ERROR("Not support type {}", value.Type());
      break;
  }
}
}  // namespace chunk
}  // namespace amdb