#include "sql/optimizer/rules/enumerable_resultset_rule.h"

#include <sql/plan/result_set.h>

namespace amdb::plan {
class PhysicalResultSet;
}
namespace amdb {
namespace opt {
EnumerableResultSetRule::EnumerableResultSetRule() : Rule("EnumerableResultSetRule") {}

std::shared_ptr<RuleMatcher> EnumerableResultSetRule::Matcher() {
  std::size_t pick_to = 0;
  std::vector<std::shared_ptr<RuleMatcher>> child;
  child.emplace_back(std::make_unique<IgnoreMany>());

  return std::make_shared<MatchAndPickNode>(&pick_to, child, plan::RelOptNodeType::kLogicalResultSet);
};

std::vector<plan::RelOptNode*> EnumerableResultSetRule::Apply(
    const Optimizer* optimizer, const std::unordered_map<std::size_t, plan::RelOptNode*>& input) {
  plan::PhysicalResultSet* result_set = new plan::PhysicalResultSet();
  for (const auto& child : input.begin()->second->GetInputs()) {
    result_set->AddInput(child);
  }
  return {result_set};
}

}  // namespace opt
}  // namespace amdb