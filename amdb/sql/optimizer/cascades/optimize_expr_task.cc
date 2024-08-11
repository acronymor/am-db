#include "sql/optimizer/cascades/optimize_expr_task.h"

#include <common/log.h>

#include <format>

#include "sql/optimizer/cascades/apply_rule_task.h"
#include "sql/optimizer/cascades/explore_group_task.h"

namespace amdb {
namespace opt {
bool TopMatches(RuleMatcher* matcher, plan::RelOptNode* node) {
  bool res = false;

  switch (matcher->GetMatcherType()) {
    case RuleMatcherType::MatchAndPickNode: {
      MatchAndPickNode* match = dynamic_cast<MatchAndPickNode*>(matcher);
      res = match->GetType() == node->GetType();
      break;
    };
    case RuleMatcherType::MatchNode: {
      MatchNode* match = dynamic_cast<MatchNode*>(matcher);
      res = match->GetType() == node->GetType();
      break;
    };
    default:
      ERROR("IR should have root node of match");
      break;
  }

  return res;
}

OptimizeExprTask::OptimizeExprTask(const ExprId expr_id, const bool exploring)
    : expr_id_(expr_id), exploring_(exploring) {}

std::vector<std::unique_ptr<Task>> OptimizeExprTask::execute(Cascades* optimizer) {
  const RelMemoNodeRef node_ref = optimizer->GetExprMemoNode(this->expr_id_);
  TRACE("event=task_begin, task=optimize_expr, expr_id={}", this->expr_id_);
  std::vector<std::unique_ptr<Task>> tasks;

  for (std::size_t rule_id = 0; rule_id < optimizer->Rules().size(); rule_id++) {
    if (optimizer->IsRuleFired(this->expr_id_, rule_id)) {
      continue;
    }

    if (this->exploring_) {
      continue;
    }

    if (TopMatches(optimizer->Rules()[rule_id].rule->Matcher(), node_ref->node)) {
      tasks.emplace_back(std::make_unique<ApplyRuleTask>(rule_id, this->expr_id_, this->exploring_));
      for (const GroupId& input_group_id : node_ref->children) {
        tasks.emplace_back(std::make_unique<ExploreGroupTask>(input_group_id));
      }
    }
  }

  TRACE("event=task_finish, task=optimize_expr, expr_id={}", this->expr_id_);
  return tasks;
};

std::string OptimizeExprTask::describe() {
  std::string s = std::format("optimizer_expr {}", this->expr_id_);
  return s;
};
}  // namespace opt
}  // namespace amdb