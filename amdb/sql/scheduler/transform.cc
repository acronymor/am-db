#include "sql/scheduler/transform.h"

namespace amdb {
namespace scheduler {
IExecutor::State ITransform::Prepare() {
  input_chunk_ = inputs_[0].PullData();
  outputs_[0].PushData(output_chunk_);
  return IExecutor::kNeedData;
}

Status ITransform::Work() { return Transform(input_chunk_, output_chunk_); }
}  // namespace scheduler
}  // namespace amdb
