#include "sql/optimizer/rules/enumerable_resultset_rule.h"

#include <sql/optimizer/optimizer.h>
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

plan::RelOptNode* EnumerableResultSetRule::Apply(const Optimizer* optimizer, plan::RelOptNode* node) {
  plan::LogicalResultSet* logical_table_scan = dynamic_cast<plan::LogicalResultSet*>(node);
  plan::PhysicalResultSet* physical_result_set = optimizer->stmt_ctx->arena->CreateObject<plan::PhysicalResultSet>();
  return physical_result_set;
}

}  // namespace opt
}  // namespace amdb