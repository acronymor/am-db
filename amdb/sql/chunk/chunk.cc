#include "sql/chunk/chunk.h"

#include "sql/codec/rc_codec.h"

namespace amdb {
namespace chunk {

Chunk::Chunk(Arena* arena, RowDescriptor* row_desc)
    : arena_(arena), row_desc_{row_desc} {}

uint32_t Chunk::Size() const { return select_.size(); }

// convert kv to chunk
Status Chunk::PullData(schema::TableInfo* table_info,
                       std::vector<std::string>& values) {
  cursor_ = 0;
  max_capacity_ = 1024;

  for (size_t i = 0; i < values.size(); i++) {
    chunk::Row* row = arena_->CreateObject<chunk::Row>(arena_, row_desc_);
    codec::DecodeRow(arena_, table_info, &values.at(i), row);
    select_.emplace_back(row);
  }

  return Status::C_OK;
}

void Chunk::AddRow(Row* row) { select_.push_back(row); }

RowDescriptor* Chunk::GetRowDesc() { return row_desc_; }

}  // namespace chunk
}  // namespace amdb