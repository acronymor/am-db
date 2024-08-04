#include "sql/optimizer/cascades/optimize_group_task.h"

#include <cstdint>
#include <format>

#include "cascades.h"
#include "common/log.h"
#include "optimize_expr_task.h"
#include "optimize_inputs_task.h"

namespace amdb {
namespace opt {
OptimizeGroupTask::OptimizeGroupTask(const GroupId& group_id) : group_id_(group_id) {}

std::vector<std::unique_ptr<Task>> OptimizeGroupTask::execute(Cascades* optimizer) {
  TRACE("event=task_begin, task=optimize_group, group_id={}", this->group_id_);
  const std::optional<GroupInfo>& group_info = optimizer->GetGroupInfo(this->group_id_);
  if (group_info.has_value() && group_info->winner != std::nullopt) {
    TRACE("event=task_finish, task=optimize_group");
    return {};
  }

  const std::vector<ExprId>& exprs = optimizer->GetAllExprsInGroup(this->group_id_);
  std::vector<std::unique_ptr<Task>> tasks;
  tasks.reserve(exprs.size());

  for (const auto& expr : exprs) {
    const RelMemoNodeRef& node_ref = optimizer->GetExprMemoNode(expr);
    if (node_ref->node->IsLogical()) {
      tasks.emplace_back(std::make_unique<OptimizeExprTask>(expr, false));
    }
  }

  for (const auto& expr : exprs) {
    const RelMemoNodeRef& node_ref = optimizer->GetExprMemoNode(expr);
    if (node_ref->node->IsPhysical()) {
      tasks.emplace_back(std::make_unique<OptimizeInputsTask>(expr, std::nullopt, true));
    }
  }

  TRACE("event=task_finish, task=optimize_group, group_id={}, exprs_cnt={}", this->group_id_, exprs.size());
  return tasks;
}

std::string OptimizeGroupTask::describe() {
  std::string s = std::format("event=task_finish, task=optimize_group, group_id={}", this->group_id_);
  return s;
}
}  // namespace opt
}  // namespace amdb