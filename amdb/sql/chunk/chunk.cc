#include "sql/chunk/chunk.h"
#include "common/log.h"

namespace amdb {
namespace chunk {

Chunk::Chunk(Arena* arena, RowDescriptor* row_desc)
    : arena_(arena), row_desc_{row_desc} {}

Chunk& Chunk::operator=(Chunk&& rsh) noexcept {
  select_ = std::move(rsh.select_);
  cursor_ = rsh.cursor_;
  row_desc_ = rsh.row_desc_;
  columns_ = std::move(rsh.columns_);
  max_capacity_ = rsh.max_capacity_;
  cur_capacity_ = rsh.cur_capacity_;
  arena_ = std::move(rsh.arena_);
}

uint32_t Chunk::Size() const { return select_.size(); }

// convert index kv to chunk
Status Chunk::PullIndexData(
    std::vector<std::pair<std::string, std::string>>& data_segment) {
  return C_OK;
}

// convert data kv to chunk
Status Chunk::PullKvData(
    std::vector<std::pair<std::string, std::string>>& data_segment) {
  for(auto& entry: data_segment) {
    INFO("key={}, value={}", entry.first, entry.second);
  }
  return Status::C_OK;
}

void Chunk::AddRow(Row* row) {
  RowDescriptor* row_desc = row->desc;
  /*
  for (ColumnDescriptor* col_desc : row_desc->GetAllColumnDesc()) {
    Column* column = arena_->CreateObject<Column>(arena_, col_desc, cur_capacity_);
    column->SetExprValue(row_desc->ID(), row->GetColValue(row_desc->ID(), col_desc->ID()));
    columns_.push_back(column);
  }
   */
  select_.push_back(row);
}

}  // namespace chunk
}  // namespace amdb