#include "sql/optimizer/rules/enumerable_resultset_rule.h"

#include <sql/plan/result_set.h>

namespace amdb::plan {
class PhysicalResultSet;
}
namespace amdb {
namespace opt {
EnumerableResultSetRule::EnumerableResultSetRule(Arena* arena) : Rule(arena, "EnumerableResultSetRule") {}

RuleMatcher* EnumerableResultSetRule::Matcher() {
  std::size_t pick_to = 0;
  std::vector<RuleMatcher*> child;
  child.emplace_back(this->arena_->CreateObject<IgnoreMany>());
  return this->arena_->CreateObject<MatchAndPickNode>(&pick_to, child, plan::RelOptNodeType::kLogicalResultSet);
};

plan::RelOptNode* EnumerableResultSetRule::Apply(plan::RelOptNode* node) {
  plan::LogicalResultSet* logical_table_scan = dynamic_cast<plan::LogicalResultSet*>(node);
  plan::PhysicalResultSet* physical_result_set = this->arena_->CreateObject<plan::PhysicalResultSet>();
  return physical_result_set;
}

}  // namespace opt
}  // namespace amdb