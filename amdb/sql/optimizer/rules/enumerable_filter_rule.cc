#include "sql/optimizer/rules/enumerable_filter_rule.h"

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

std::vector<plan::RelOptNode*> EnumerableFilterRule::Apply(
    const Optimizer* optimizer, const std::unordered_map<std::size_t, plan::RelOptNode*>& input) {
  plan::PhysicalFilter* filter = new plan::PhysicalFilter();
  for (const auto& child : input.begin()->second->GetInputs()) {
    filter->AddInput(child);
  }
  return {filter};
}

}  // namespace opt
}  // namespace amdb