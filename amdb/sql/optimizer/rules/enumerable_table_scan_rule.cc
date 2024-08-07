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

std::vector<plan::RelOptNode*> EnumerableTableScanRule::Apply(const Optimizer* optimizer, plan::RelOptNode* input) {
  plan::PhysicalTableScan* scan = new plan::PhysicalTableScan();
  for (const auto& item : input->GetInputs()) {
    scan->AddInput(item);
  }
  return {scan};
}
};  // namespace opt
};  // namespace amdb
