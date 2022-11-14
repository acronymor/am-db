#include "sql/executor/filter_executor.h"

namespace amdb {
namespace executor {

Status FilterExecutor::Open() { return Status::C_OK; }

Status FilterExecutor::Close() { return Status::C_OK; }

Status FilterExecutor::Transform(chunk::Chunk *input_chunk,
                                 chunk::Chunk *output_chunk) {
  output_chunk->select_ = std::move(input_chunk->select_);
  output_chunk->cursor_ = input_chunk->cursor_;
  output_chunk->row_desc_ = input_chunk->row_desc_;
  output_chunk->columns_ = std::move(input_chunk->columns_);
  output_chunk->max_capacity_ = input_chunk->max_capacity_;
  output_chunk->cur_capacity_ = input_chunk->cur_capacity_;
  output_chunk->arena_ = std::move(input_chunk->arena_);
  return C_OK;
}
}  // namespace executor
}  // namespace amdb