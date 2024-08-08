#include "sql/optimizer/rules/enumerable_filter_rule.h"

#include <sql/optimizer/optimizer.h>

#include "absl/time/clock.h"
#include "sql/plan/filter.h"

namespace amdb {
namespace opt {
EnumerableFilterRule::EnumerableFilterRule() : Rule("EnumerableFilterRule") {}

std::shared_ptr<RuleMatcher> EnumerableFilterRule::Matcher() {
  std::size_t pick_to = 0;
  std::vector<std::shared_ptr<RuleMatcher>> child;
  child.emplace_back(std::make_unique<IgnoreMany>());

  return std::make_shared<MatchAndPickNode>(&pick_to, child, plan::RelOptNodeType::kLogicalFilter);
};

plan::RelOptNode* EnumerableFilterRule::Apply(const Optimizer* optimizer, plan::RelOptNode* node) {
  plan::LogicalFilter* logical_filter = dynamic_cast<plan::LogicalFilter*>(node);
  plan::PhysicalFilter* physical_filter = optimizer->stmt_ctx->arena->CreateObject<plan::PhysicalFilter>();
  physical_filter->SetConditions(logical_filter->GetConditions());
  return physical_filter;
}

}  // namespace opt
}  // namespace amdb