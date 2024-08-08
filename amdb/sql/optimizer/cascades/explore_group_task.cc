#include "sql/optimizer/cascades/explore_group_task.h"

#include <common/log.h>

#include "optimize_expr_task.h"

namespace amdb {
namespace opt {
ExploreGroupTask::ExploreGroupTask(const GroupId& group_id) : group_id_(group_id) {}

std::vector<std::unique_ptr<Task>> ExploreGroupTask::execute(Cascades* optimizer) {
  TRACE("event=task_begin, task=explore_group, group_id={}", this->group_id_);
  if (optimizer->IsGroupExplored(this->group_id_)) {
    TRACE("event=task_finish, task=explore_group, group_id={} already explored, skippint", this->group_id_);
    return {};
  }

  const std::vector<ExprId> exprs = optimizer->GetAllExprsInGroup(this->group_id_);
  std::vector<std::unique_ptr<Task>> tasks;
  tasks.reserve(exprs.size());

  for (const auto& expr : exprs) {
    if (optimizer->GetExprMemoNode(expr)->node->IsLogical()) {
      tasks.emplace_back(std::make_unique<OptimizeExprTask>(expr, true));
    }
  }

  optimizer->MarkGroupExplored(this->group_id_);

  TRACE("event=task_finish, task=explore_group, group_id={} expand group, exprs_cnt={}", this->group_id_, exprs.size());
  return tasks;
}

std::string ExploreGroupTask::describe() {
  std::string s = std::format("explore_group {}", this->group_id_);
  return s;
}
}  // namespace opt
}  // namespace amdb