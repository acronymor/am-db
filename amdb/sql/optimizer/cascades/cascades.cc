#include "sql/optimizer/cascades/cascades.h"

#include <sql/optimizer/costs/adaptive_cost.h>
#include <sql/parser/sql_lex.flex.h>

#include "common/assert.h"
#include "placeholder.h"
#include "sql/optimizer/cascades/optimize_group_task.h"
#include "sql/optimizer/cascades/task.h"
#include "sql/optimizer/rules/rules.h"

namespace amdb {
namespace opt {
Cascades::Cascades(StatementContext* stmt_ctx) : stmt_ctx_(stmt_ctx) {
  this->memo_ = std::make_unique<Memo>();
  this->cost_ = std::make_shared<AdaptiveCost>();
  InitCascadesRules(&this->rules_);
};

plan::RelOptNode* Cascades::FindBestExp() {
  const GroupId group_id = this->initMemo(this->stmt_ctx_->logical_plan);
  const Status status = this->fireOptimizeTask(group_id);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  plan::RelOptNode* root = this->stepOptmizeRel(group_id);
  return root;
};

GroupId Cascades::initMemo(plan::RelOptNode* node) const {
  const auto [group_id, expr_id] = this->memo_->InitMemo(node);
  return group_id;
}

Status Cascades::fireOptimizeTask(const GroupId& group_id) {
  this->tasks_.push(std::make_unique<OptimizeGroupTask>(group_id));

  while (!this->tasks_.empty()) {
    const std::unique_ptr<Task>& task = this->tasks_.top();
    std::vector<std::unique_ptr<Task>> tasks = task->execute(this);
    this->tasks_.pop();
    for (auto& item : tasks) {
      this->tasks_.push(std::move(item));
    }
  }

  return Status::C_OK;
}

plan::RelOptNode* Cascades::stepOptmizeRel(const GroupId& group_id) const { return this->memo_->GetBestPlan(group_id); }

std::pair<GroupId, ExprId> Cascades::AddGroupExpr(plan::RelOptNode* expr, const std::optional<GroupId>& group_id) {
  return this->memo_->AddNewGroupExpr(expr, group_id);
};

std::optional<GroupId> Cascades::GetGroupId(const ExprId& expr_id) { return this->memo_->GetGroupId(expr_id); };

std::optional<GroupInfo> Cascades::GetGroupInfo(const GroupId& group_id) { return this->memo_->GetGroupInfo(group_id); }

void Cascades::UpdateGroupInfo(const GroupId& group_id, const GroupInfo& group_info) {
  this->memo_->UpdateGroupInfo(group_id, group_info);
}

std::vector<ExprId> Cascades::GetAllExprsInGroup(const GroupId& group_id) const {
  return this->memo_->GetAllExprsInGroup(group_id);
}

RelMemoNodeRef Cascades::GetExprMemoNode(const ExprId& expr_id) const { return this->memo_->GetExprMemoNode(expr_id); }

const std::vector<RuleWrapper>& Cascades::Rules() const { return this->rules_; }

bool Cascades::IsRuleFired(const ExprId& group_expr_id, const RuleId& rule_id) const {
  if (!this->fired_rules_.contains(group_expr_id)) {
    return false;
  }

  const std::unordered_set<RuleId>& fired_rules = this->fired_rules_.at(group_expr_id);
  if (!fired_rules.contains(rule_id)) {
    return false;
  }

  return true;
};

void Cascades::MarkRuleFired(const ExprId& group_expr_id, const RuleId& rule_id) {
  this->fired_rules_[group_expr_id].insert(rule_id);
}

bool Cascades::IsGroupExplored(const GroupId& group_id) const { return this->explored_group_.contains(group_id); }

void Cascades::MarkGroupExplored(const GroupId& group_id) { this->explored_group_.insert(group_id); }

bool Cascades::IsRuleDisabled(const RuleId& rule_id) const { return this->disable_rules_.contains(rule_id); }

std::shared_ptr<CostModel> Cascades::Cost() const { return this->cost_; }
};  // namespace opt
}  // namespace amdb
