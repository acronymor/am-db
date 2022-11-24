#include "sql/executor/filter_executor.h"

namespace amdb {
namespace executor {

Status FilterExecutor::Open() { return Status::C_OK; }

Status FilterExecutor::Close() { return Status::C_OK; }

Status FilterExecutor::Transform(chunk::Chunk *input_chunk,
                                 chunk::Chunk *output_chunk) {
  *output_chunk = std::move(*input_chunk);
  return C_OK;
}
}  // namespace executor
}  // namespace amdb