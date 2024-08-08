#include "sql/optimizer/rules/enumerable_table_scan_rule.h"

#include <sql/optimizer/optimizer.h>

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

plan::RelOptNode* EnumerableTableScanRule::Apply(const Optimizer* optimizer, plan::RelOptNode* node) {
  plan::LogicalTableScan* logical_table_scan = dynamic_cast<plan::LogicalTableScan*>(node);
  plan::PhysicalTableScan* physical_table_scan = optimizer->stmt_ctx->arena->CreateObject<plan::PhysicalTableScan>();

  physical_table_scan->SetTable(logical_table_scan->GetTable());
  physical_table_scan->PrimaryFilters()->assign(logical_table_scan->PrimaryFilters()->begin(), logical_table_scan->PrimaryFilters()->end());
  physical_table_scan->PrimaryRanges()->assign(logical_table_scan->PrimaryRanges()->begin(), logical_table_scan->PrimaryRanges()->end());
  physical_table_scan->TableFilters()->assign(logical_table_scan->TableFilters()->begin(), logical_table_scan->TableFilters()->end());
  return physical_table_scan;
}
};  // namespace opt
};  // namespace amdb
