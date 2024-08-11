#include "sql/optimizer/rules/enumerable_filter_rule.h"

#include "absl/time/clock.h"
#include "sql/plan/filter.h"

namespace amdb {
namespace opt {
EnumerableFilterRule::EnumerableFilterRule(Arena* arena) : Rule(arena, "EnumerableFilterRule") {}

RuleMatcher* EnumerableFilterRule::Matcher() {
  std::size_t pick_to = 0;
  std::vector<RuleMatcher*> child;
  child.emplace_back(this->arena_->CreateObject<IgnoreMany>());
  return this->arena_->CreateObject<MatchAndPickNode>(&pick_to, child, plan::RelOptNodeType::kLogicalFilter);
};

plan::RelOptNode* EnumerableFilterRule::Apply(plan::RelOptNode* node) {
  plan::LogicalFilter* logical_filter = dynamic_cast<plan::LogicalFilter*>(node);
  plan::PhysicalFilter* physical_filter = this->arena_->CreateObject<plan::PhysicalFilter>();
  physical_filter->SetConditions(logical_filter->GetConditions());
  return physical_filter;
}

}  // namespace opt
}  // namespace amdb