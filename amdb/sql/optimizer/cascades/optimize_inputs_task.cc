#include "sql/optimizer/cascades/optimize_inputs_task.h"

#include <cstdint>
#include <format>

#include "cascades.h"
#include "common/log.h"
#include "sql/optimizer/cascades/optimize_group_task.h"

namespace amdb {
namespace opt {
std::vector<plan::Cost> FirstInvoke(Cascades* optimizer, const std::vector<GroupId>& children) {
  plan::Cost zero_cost = optimizer->Cost()->Zero();
  std::vector<plan::Cost> input_cost;
  input_cost.reserve(children.size());

  for (const GroupId& child : children) {
    const GroupInfo group = optimizer->GetGroupInfo(child).value();
    if (group.winner) {
      if (!group.winner->impossible) {
        input_cost.push_back(group.winner->cost);
        continue;
      }
    }
    input_cost.push_back(zero_cost);
  }

  return input_cost;
}

bool IsUpdateWinner(Cascades* optimizer, const plan::Cost& cost_so_far, const GroupId& group_id) {
  const GroupInfo group_info = optimizer->GetGroupInfo(group_id).value();
  bool update_cost = false;

  const auto& winner = group_info.winner;

  if (winner) {
    if (winner->impossible || winner->cost > cost_so_far) {
      update_cost = true;
    }
  } else {
    update_cost = true;
  }

  return update_cost;
}

OptimizeInputsTask::OptimizeInputsTask(const ExprId& expr_id, const std::optional<ContinueTask>& cont, bool pruning)
    : expr_id_(expr_id), continue_from_(cont), pruning_(pruning) {}

std::vector<std::unique_ptr<Task>> OptimizeInputsTask::execute(Cascades* optimizer) {
  std::string continue_from_group_idx =
      std::format("{}", this->continue_from_.has_value() ? std::to_string(this->continue_from_->next_group_idx) : "");
  TRACE("event=task_begin, task=optimize_inputs, expr_id={}, continue_from={}", this->expr_id_,
        continue_from_group_idx);
  const RelMemoNodeRef& expr = optimizer->GetExprMemoNode(this->expr_id_);
  const std::vector<GroupId>& children_group_ids = expr->children;
  const std::shared_ptr<CostModel>& cost = optimizer->Cost();

  if (this->continue_from_) {
    if (this->continue_from_->next_group_idx < children_group_ids.size()) {
      const GroupId& children_group_id = children_group_ids[this->continue_from_->next_group_idx];
      const GroupInfo group_info = optimizer->GetGroupInfo(children_group_id).value();
      bool has_full_winner = false;

      if (group_info.winner) {
        if (!group_info.winner->impossible) {
          has_full_winner = true;
        }
      }
      if (!has_full_winner) {
        if (!this->continue_from_->return_from_optimize_group) {
          TRACE("event=task_yield, task=optimize_inputs, expr_id={}, group_idx={}", this->expr_id_,
                this->continue_from_->next_group_idx);
          std::vector<std::unique_ptr<Task>> tasks;
          ContinueTask task(this->continue_from_->next_group_idx, this->continue_from_->input_cost, true);
          tasks.emplace_back(std::make_unique<OptimizeInputsTask>(this->expr_id_, task, this->pruning_));
          tasks.emplace_back(std::make_unique<OptimizeGroupTask>(children_group_id));
          return tasks;
        }
      }

      TRACE("event=task_yield, task=optimize_inputs, expr_id={}, group_idx={}", this->expr_id_,
            this->continue_from_->next_group_idx);
      std::vector<std::unique_ptr<Task>> tasks;
      ContinueTask task(this->continue_from_->next_group_idx + 1, continue_from_->input_cost, false);
      tasks.emplace_back(std::make_unique<OptimizeInputsTask>(this->expr_id_, task, this->pruning_));
      return tasks;
    } else {
      const GroupId group_id = optimizer->GetGroupId(this->expr_id_).value();
      // const plan::Cost cost_so_far = cost->Sum(expr->node->FindLocalCost(), this->continue_from_->input_cost);
      const plan::Cost& cost_so_far = cost->Sum(cost->Zero(), this->continue_from_->input_cost);
      if (IsUpdateWinner(optimizer, cost_so_far, group_id)) {
        Winner winner = {.impossible = false, .expr_id = this->expr_id_, .cost = cost_so_far};
        GroupInfo group_info{winner};
        optimizer->UpdateGroupInfo(group_id, group_info);
      }
      TRACE("event=task_finish, task=optimize_inputs, expr_id={}", this->expr_id_);
      return {};
    }
  } else {
    const std::vector<plan::Cost>& input_cost = FirstInvoke(optimizer, children_group_ids);
    TRACE("event=task_yield, task=optimize_inputs, expr_id={}", this->expr_id_);
    std::vector<std::unique_ptr<Task>> tasks;
    ContinueTask task(0, input_cost, false);
    tasks.emplace_back(std::make_unique<OptimizeInputsTask>(this->expr_id_, task, this->pruning_));
    return tasks;
  }
}

std::string OptimizeInputsTask::describe() {
  std::string s = std::format("optimize_input {}", this->expr_id_);
  return s;
}
}  // namespace opt
}  // namespace amdb