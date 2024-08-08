#pragma once

#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "sql/common/statuscode.h"
#include "sql/context/statement_context.h"
#include "sql/optimizer/cascades/memo.h"
#include "sql/optimizer/cascades/task.h"
#include "sql/optimizer/costs/cost_model.h"
#include "sql/optimizer/optimizer.h"
#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class Cascades final : public Optimizer {
 public:
  explicit Cascades(StatementContext* stmt_ctx);

  plan::RelOptNode* FindBestExp() override;

 private:
  GroupId initMemo(plan::RelOptNode* node) const;
  plan::RelOptNode* stepOptmizeRel(const GroupId& group_id) const;
  Status fireOptimizeTask(const GroupId& group_id);

 public:
  std::pair<GroupId, ExprId> AddGroupExpr(plan::RelOptNode* expr, const std::optional<GroupId>& group_id);
  std::optional<GroupId> GetGroupId(const ExprId& expr_id);
  std::optional<GroupInfo> GetGroupInfo(const GroupId& group_id);
  void UpdateGroupInfo(const GroupId& group_id, const GroupInfo& group_info);
  std::vector<ExprId> GetAllExprsInGroup(const GroupId& group_id) const;
  RelMemoNodeRef GetExprMemoNode(const ExprId& expr_id) const;
  const std::vector<RuleWrapper>& Rules() const;
  bool IsRuleFired(const ExprId& group_expr_id, const RuleId& rule_id) const;
  void MarkRuleFired(const ExprId& group_expr_id, const RuleId& rule_id);
  bool IsGroupExplored(const GroupId& group_id) const;
  void MarkGroupExplored(const GroupId& group_id);
  bool IsRuleDisabled(const RuleId& rule_id) const;
  std::shared_ptr<CostModel> Cost() const;

 private:
  std::unique_ptr<Memo> memo_;
  std::stack<std::unique_ptr<Task>> tasks_;
  std::vector<RuleWrapper> rules_;
  std::unordered_map<ExprId, std::unordered_set<RuleId>> fired_rules_;
  std::unordered_set<RuleId> disable_rules_;

  std::shared_ptr<CostModel> cost_;

  std::unordered_set<GroupId> explored_group_;

  StatementContext* stmt_ctx_;
};

};  // namespace opt
}  // namespace amdb
