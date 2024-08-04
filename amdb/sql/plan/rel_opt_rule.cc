#include "sql/plan/rel_opt_rule.h"

#include "sql/plan/insert.h"
#include "sql/plan/table_scan.h"

namespace amdb {
namespace plan {
RelOptRule::RelOptRule(const RelOptRule& rule) {
  std::cout << "RelOptRule::RelOptRule deep copy RelOptRule" << std::endl;
}

PhysicalTableScanRule::PhysicalTableScanRule() : RelOptRule(new LogicalTableScan(), new PhysicalTableScan(), 0) {}
RelOptRuleType PhysicalTableScanRule::GetType() { return RelOptRuleType::kPhysicalTableScanRule; }
uint64_t PhysicalTableScanRule::GetId() { return 0; }
std::string PhysicalTableScanRule::GetName() { return "PhysicalTableScanRule"; }
RelOptNode* PhysicalTableScanRule::Substitute(RelOptNode* before, RelTraitSet* trait_set) {
  RelOptNode* after = GetSubstitute();
  for (RelOptNode* node : before->GetInputs()) {
    after->AddInput(node);
  }
  return after;
}

PhysicalFilterRule::PhysicalFilterRule() : RelOptRule(new LogicalFilter(), new PhysicalFilter(), 0) {}
RelOptRuleType PhysicalFilterRule::GetType() { return RelOptRuleType::kPhysicalFilterRule; }
uint64_t PhysicalFilterRule::GetId() { return 0; }
std::string PhysicalFilterRule::GetName() { return std::string(); }
RelOptNode* PhysicalFilterRule::Substitute(RelOptNode* before, RelTraitSet* trait_set) {
  RelOptNode* after = GetSubstitute();
  for (RelOptNode* node : before->GetInputs()) {
    after->AddInput(node);
  }
  return after;
}

PhysicalResultSetRule::PhysicalResultSetRule() : RelOptRule(new LogicalResultSet(), new PhysicalResultSet(), 0) {}
RelOptNode* PhysicalResultSetRule::Substitute(RelOptNode* before, RelTraitSet* trait_set) {
  RelOptNode* after = GetSubstitute();
  after->AddInput(before->GetInput(0));
  return after;
}

RelOptRuleType PhysicalResultSetRule::GetType() { return RelOptRuleType::kPhysicalResultSetRule; }
uint64_t PhysicalResultSetRule::GetId() { return 0; }
std::string PhysicalResultSetRule::GetName() { return "PhysicalResultSetRule"; }
}  // namespace plan
}  // namespace amdb