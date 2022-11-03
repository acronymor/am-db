#include "sql/scheduler/sink.h"

namespace amdb {
namespace scheduler {

IExecutor::State ISink::Prepare() {
  chunk_ = inputs_[0].PullData();
  return IExecutor::kFinished;
}

Status ISink::Work() {
  return Consume(chunk_);
}
}  // namespace scheduler
}  // namespace amdb
