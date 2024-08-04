#include "sql/plan/table_scan.h"

namespace amdb {
namespace plan {
uint64_t TableScan::GetId() { return RelOptNode::GetId(); }

void TableScan::SetTable(RelOptTable* table) { table_ = table; }
RelOptTable* TableScan::GetTable() const { return table_; }
std::vector<IndexRange*>* TableScan::PrimaryRanges() { return &primary_ranges; }
std::vector<expr::ExprNode*>* TableScan::PrimaryFilters() { return &primary_filters; }
std::vector<expr::ExprNode*>* TableScan::TableFilters() { return &table_filters; }

const std::string LogicalTableScan::GetName() { return "LogicalTableScan"; }
uint64_t LogicalTableScan::GetArity() { return 0; }
uint64_t LogicalTableScan::Hash() { return 0; }
uint64_t LogicalTableScan::GetId() { return TableScan::GetId(); }
RelOptNodeType LogicalTableScan::GetType() { return RelOptNodeType::kLogicalTableScan; }
plan::Cost LogicalTableScan::FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}

const std::string PhysicalTableScan::GetName() { return "PhysicalTableScan"; }
uint64_t PhysicalTableScan::GetArity() { return 0; }
uint64_t PhysicalTableScan::Hash() { return 0; }
uint64_t PhysicalTableScan::GetId() { return TableScan::GetId(); }
RelOptNodeType PhysicalTableScan::GetType() { return RelOptNodeType::kPhysicalTableScan; }
plan::Cost PhysicalTableScan::FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}
}  // namespace plan
}  // namespace amdb