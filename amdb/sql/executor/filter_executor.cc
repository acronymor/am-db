#include "sql/executor/filter_executor.h"

namespace amdb {
namespace executor {

Status FilterExecutor::Open() {
  for (auto &condition : filter_plan_->GetConditions()) {
    // RETURN_ERR_NOT_OK(condition->Open(ctx_));
  }
  return Status::C_OK;
}

Status FilterExecutor::Close() {
  for (auto &condition : filter_plan_->GetConditions()) {
    // RETURN_ERR_NOT_OK(condition->Close(ctx_));
  }
  return Status::C_OK;
}

Status FilterExecutor::Transform(chunk::Chunk *input_chunk,
                                 chunk::Chunk *output_chunk) {
  *output_chunk = std::move(*input_chunk);
  return C_OK;
}
}  // namespace executor
}  // namespace amdb