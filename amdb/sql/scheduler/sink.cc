#include "sql/scheduler/sink.h"

namespace amdb {
namespace scheduler {

IExecutor::State ISink::Prepare() {
  if (inputs_.size() == 0) {
    return IExecutor::kFinished;
  }

  if (inputs_[0].IsFinished()) {
    return IExecutor::kFinished;
  }

  chunk_ = inputs_[0].PullData();
  return IExecutor::kReady;
}

Status ISink::Work() { return Consume(chunk_); }
}  // namespace scheduler
}  // namespace amdb
