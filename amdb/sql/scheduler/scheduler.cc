#include "sql/scheduler/scheduler.h"

namespace amdb {
namespace scheduler {
Status ExecutePlan(StatementContext* stmt_ctx) {
  Scheduler scheduler(stmt_ctx);
  Status status = scheduler.Run();
  if (status != Status::C_OK) {
    ERROR("scheduler execute failed while run plan");
    return status;
  }
  return Status::C_OK;
}

// TODO implement single-thread to multi-thread concurrency
Status Scheduler::Run() {
  RETURN_ERR_NOT_OK(graph_.Init());
  RETURN_ERR_NOT_OK(graph_.Open());

  for (auto& node : graph_.Nodes()) {
    pending_tasks_.push(std::make_shared<Task>(node.get()));
  }

  // TODO make limit to the number of tasks
  do {
    while (!executing_tasks_.empty()) {
      std::shared_ptr<Task> task = executing_tasks_.front();
      executing_tasks_.pop();
      RETURN_ERR_NOT_OK(graph_.Prepare(task->node));
      RETURN_ERR_NOT_OK(graph_.Work(task->node));
    }

    while (!pending_tasks_.empty()) {
      std::shared_ptr<Task> task = pending_tasks_.front();
      pending_tasks_.pop();
      executing_tasks_.push(task);
    }
  } while (!executing_tasks_.empty());

  RETURN_ERR_NOT_OK(graph_.Close());
  RETURN_ERR_NOT_OK(graph_.Finish());
  return Status::C_OK;
}
}  // namespace scheduler
}  // namespace amdb