#include "sql/optimizer/cascades/apply_rule_task.h"

#include <common/log.h>

#include "sql/optimizer/cascades/cascades.h"
#include "sql/optimizer/cascades/optimize_expr_task.h"
#include "sql/optimizer/cascades/optimize_inputs_task.h"
#include "sql/optimizer/cascades/placeholder.h"

namespace amdb {
namespace opt {
std::vector<plan::RelOptNode*> MatchAndPickGroup(Cascades* optimizer, const GroupId& group_id,
                                                 const std::shared_ptr<RuleMatcher>& matcher);
std::vector<plan::RelOptNode*> MatchAndPickExpr(Cascades* optimizer, const ExprId& expr_id,
                                                const std::shared_ptr<RuleMatcher>& matcher);

auto MatchNodeInner(Cascades* optimizer, const RelMemoNodeRef& node, std::optional<std::size_t> pick_to,
                    const std::vector<std::shared_ptr<RuleMatcher>>& children) -> std::vector<plan::RelOptNode*> {
  bool should_end = false;
  std::vector<plan::RelOptNode*> picks;

  for (std::size_t idx = 0; idx < children.size(); idx++) {
    const std::shared_ptr<RuleMatcher>& child = children[idx];
    switch (child->GetMatcherType()) {
      case RuleMatcherType::IgnoreOne:
        break;
      case RuleMatcherType::IgnoreMany: {
        should_end = true;
        break;
      };
      case RuleMatcherType::PickOne: {
        GroupId group_id = node->children[idx];
        bool expand = dynamic_cast<PickOne*>(child.get())->GetExpand();
        DEBUG("group_id={}, expand={}", group_id, expand);
        break;
      };
      case RuleMatcherType::PickMany: {
        should_end = true;
        break;
      };
      default: {
        const auto new_picks = MatchAndPickGroup(optimizer, node->children[idx], child);
        for (const auto& new_pick : new_picks) {
          picks.emplace_back(new_pick);
        }
      };
    }
  }

  if (pick_to) {
    plan::RelOptNode* root = new PlaceHolder(node->node, 0);
    for (const auto& input : node->children) {
      root->AddInput(new PlaceHolder(nullptr, input));
    }
    picks.emplace_back(std::move(root));
  }

  return picks;
}

auto MatchAndPick(Cascades* optimizer, const RelMemoNodeRef& node, const std::shared_ptr<RuleMatcher>& matcher)
    -> std::vector<plan::RelOptNode*> {
  std::vector<plan::RelOptNode*> picks;
  switch (matcher->GetMatcherType()) {
    case RuleMatcherType::MatchAndPickNode: {
      MatchAndPickNode* pick_node = dynamic_cast<MatchAndPickNode*>(matcher.get());
      if (node->node->GetType() != pick_node->GetType()) {
        return {};
      }
      const auto tmp = MatchNodeInner(optimizer, node, std::optional(pick_node->PickTo()), pick_node->GetChildren());
      picks.insert(picks.end(), tmp.begin(), tmp.end());
      break;
    };
    case RuleMatcherType::MatchNode: {
      MatchNode* pick_node = dynamic_cast<MatchNode*>(matcher.get());
      if (node->node->GetType() != pick_node->GetType()) {
        return {};
      }
      const auto tmp = MatchNodeInner(optimizer, node, std::nullopt, pick_node->GetChildren());
      picks.insert(picks.end(), tmp.begin(), tmp.end());
      break;
    };
    default:
      ERROR("top node should be match node");
  }
  return picks;
};

auto MatchAndPickExpr(Cascades* optimizer, const ExprId& expr_id, const std::shared_ptr<RuleMatcher>& matcher)
    -> std::vector<plan::RelOptNode*> {
  const RelMemoNodeRef node_ref = optimizer->GetExprMemoNode(expr_id);
  return MatchAndPick(optimizer, node_ref, matcher);
}

auto MatchAndPickGroup(Cascades* optimizer, const GroupId& group_id, const std::shared_ptr<RuleMatcher>& matcher)
    -> std::vector<plan::RelOptNode*> {
  std::vector<plan::RelOptNode*> picks;
  for (const auto& expr_id : optimizer->GetAllExprsInGroup(group_id)) {
    const RelMemoNodeRef node_ref = optimizer->GetExprMemoNode(expr_id);
    const auto pick = MatchAndPick(optimizer, node_ref, matcher);
    picks.insert(picks.end(), pick.begin(), pick.end());
  }

  return picks;
}

ApplyRuleTask::ApplyRuleTask(const RuleId& rule_id, const ExprId& expr_id, bool exploring)
    : rule_id_(rule_id), expr_id_(expr_id), exploring_(exploring) {}

std::vector<std::unique_ptr<Task>> ApplyRuleTask::execute(Cascades* optimizer) {
  if (optimizer->IsRuleFired(this->expr_id_, this->rule_id_)) {
    return {};
  }

  if (optimizer->IsRuleDisabled(this->rule_id_)) {
    optimizer->MarkRuleFired(this->expr_id_, this->rule_id_);
    return {};
  }

  const std::vector<RuleWrapper>& rule_wrappers = optimizer->Rules();
  const std::shared_ptr<Rule>& rule = rule_wrappers[this->rule_id_].rule;
  TRACE("event=task_begin, task=apply_rule, expr_id={}, rule_id={}, rule_name={}", this->expr_id_, this->rule_id_,
        rule->Name());

  const GroupId group_id = optimizer->GetGroupId(this->expr_id_).value();
  std::vector<std::unique_ptr<Task>> tasks;

  const auto binding_exprs = MatchAndPickExpr(optimizer, this->expr_id_, rule->Matcher());
  for (const auto& expr : binding_exprs) {
    PlaceHolder* holder = dynamic_cast<PlaceHolder*>(expr);
    const auto applied = rule->Apply(optimizer, holder->GetNode());
    for (const auto& child : expr->GetInputs()) {
      applied->AddInput(child);
    }
    // TODO merge group
    const std::pair<GroupId, ExprId> entry = optimizer->AddGroupExpr(applied, group_id);
    TRACE("apply_rule expr_id={}, rule_id={}, new_expr_id={}", this->expr_id_, this->rule_id_, entry.second);
    if (applied->IsLogical()) {
      tasks.emplace_back(std::make_unique<OptimizeExprTask>(entry.second, this->exploring_));
    } else {
      tasks.emplace_back(std::make_unique<OptimizeInputsTask>(entry.second, std::nullopt, true));
    }
  }

  optimizer->MarkRuleFired(this->expr_id_, this->rule_id_);
  TRACE("event=task_finish, task=apply_rule, expr_id={}, rule_id={}", this->expr_id_, rule_id_);

  return tasks;
}

std::string ApplyRuleTask::describe() {
  std::string s = std::format("apply_rule {{rule_id: {}, expr_id: {}, exploring: {}}}", this->expr_id_, this->rule_id_,
                              this->exploring_);
  return s;
}
}  // namespace opt
}  // namespace amdb
