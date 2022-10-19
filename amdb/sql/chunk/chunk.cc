#include "sql/chunk/chunk.h"

namespace amdb {
namespace chunk {

Chunk::Chunk(Arena* arena, RowDescriptor* row_desc): arena_(arena), row_desc_{row_desc} {}

uint32_t Chunk::Size() const { return select_.size(); }

// convert index kv to chunk
Status Chunk::PullIndexData(
    std::vector<std::pair<std::string, std::string>>& data_segment) {
  return C_OK;
}

// convert data kv to chunk
Status Chunk::PullKvData(
    std::vector<std::pair<std::string, std::string>>& data_segment) {
  return Status::C_OK;
}

void Chunk::AddRow(Row* row) {
  for (ColumnDescriptor* col_desc : row->desc->GetAllColumnDesc()) {
    Column* column =
        arena_->CreateObject<Column>(arena_, col_desc, cur_capacity_);
    columns_.push_back(column);
  }
}

}  // namespace chunk
}  // namespace amdb