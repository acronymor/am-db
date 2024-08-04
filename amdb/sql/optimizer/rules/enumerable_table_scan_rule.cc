#include "sql/optimizer/rules/enumerable_table_scan_rule.h"

#include "sql/plan/table_scan.h"

namespace amdb {
namespace opt {
EnumerableTableScanRule::EnumerableTableScanRule() : Rule("EnumerableTableScanRule") {}

std::shared_ptr<RuleMatcher> EnumerableTableScanRule::Matcher() {
  std::size_t pick_to = 0;
  std::vector<std::shared_ptr<RuleMatcher>> child;
  child.emplace_back(std::make_unique<IgnoreMany>());

  return std::make_shared<MatchAndPickNode>(&pick_to, child, plan::RelOptNodeType::kLogicalTableScan);
};

std::vector<plan::RelOptNode*> EnumerableTableScanRule::Apply(
    const Optimizer* optimizer, const std::unordered_map<std::size_t, plan::RelOptNode*>& input) {
  plan::LogicalTableScan* tmp = dynamic_cast<plan::LogicalTableScan*>(input.at(0));
  plan::PhysicalTableScan* scan = new plan::PhysicalTableScan();
  /*
  for (const auto& item : tmp->GetInputs()) {
    scan->AddInput(item);
  }
  */
  return {scan};
}
};  // namespace opt
};  // namespace amdb
