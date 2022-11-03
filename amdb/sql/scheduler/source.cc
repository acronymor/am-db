#include "sql/scheduler/source.h"

namespace amdb {
namespace scheduler {
IExecutor::State ISource::Prepare() {
  outputs_.at(0).PushData(chunk_);
  return kFinished;
}

Status ISource::Work() { return Generate(chunk_); }
}  // namespace scheduler
}  // namespace amdb