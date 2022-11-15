#include "sql/executor/filter_executor.h"

namespace amdb {
namespace executor {

Status FilterExecutor::Open() { return Status::C_OK; }

Status FilterExecutor::Close() { return Status::C_OK; }

Status FilterExecutor::Transform(chunk::Chunk *input_chunk,
                                 chunk::Chunk *output_chunk) {
  output_chunk->select_ = std::move(input_chunk->select_);
  return C_OK;
}
}  // namespace executor
}  // namespace amdb