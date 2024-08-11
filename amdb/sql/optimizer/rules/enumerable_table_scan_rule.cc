#include "sql/optimizer/rules/enumerable_table_scan_rule.h"

#include "sql/plan/table_scan.h"

namespace amdb {
namespace opt {
EnumerableTableScanRule::EnumerableTableScanRule(Arena* arena) : Rule(arena, "EnumerableTableScanRule") {}

RuleMatcher* EnumerableTableScanRule::Matcher() {
  std::size_t pick_to = 0;
  std::vector<RuleMatcher*> child;
  child.emplace_back(this->arena_->CreateObject<IgnoreMany>());

  return this->arena_->CreateObject<MatchAndPickNode>(&pick_to, child, plan::RelOptNodeType::kLogicalTableScan);
};

plan::RelOptNode* EnumerableTableScanRule::Apply(plan::RelOptNode* node) {
  plan::LogicalTableScan* logical_table_scan = dynamic_cast<plan::LogicalTableScan*>(node);
  plan::PhysicalTableScan* physical_table_scan = this->arena_->CreateObject<plan::PhysicalTableScan>();

  physical_table_scan->SetTable(logical_table_scan->GetTable());
  physical_table_scan->PrimaryFilters()->assign(logical_table_scan->PrimaryFilters()->begin(),
                                                logical_table_scan->PrimaryFilters()->end());
  physical_table_scan->PrimaryRanges()->assign(logical_table_scan->PrimaryRanges()->begin(),
                                               logical_table_scan->PrimaryRanges()->end());
  physical_table_scan->TableFilters()->assign(logical_table_scan->TableFilters()->begin(),
                                              logical_table_scan->TableFilters()->end());
  return physical_table_scan;
}
};  // namespace opt
};  // namespace amdb
