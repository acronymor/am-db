#include "sql/scheduler/executor.h"

namespace amdb {
namespace scheduler {

std::string IExecutor::Name() const {
  return "UnknownExecutor";
}
}  // namespace scheduler
}  // namespace amdb