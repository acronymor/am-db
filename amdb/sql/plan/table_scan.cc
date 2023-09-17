#include "sql/plan/table_scan.h"

namespace amdb {
namespace plan {
uint64_t TableScan::GetId() { return RelOptNode::GetId(); }

RelOptCost TableScan::ComputeCost(RelOptCostFactory* factory) { return RelOptCost(table_->GetRows(), 0, 0); }
void TableScan::SetTable(RelOptTable* table) { table_ = table; }
RelOptTable* TableScan::GetTable() const { return table_; }
std::vector<IndexRange*>* TableScan::PrimaryRanges() { return &primary_ranges; }
std::vector<expr::ExprNode*>* TableScan::PrimaryFilters() { return &primary_filters; }
std::vector<expr::ExprNode*>* TableScan::TableFilters() { return &table_filters; }

const std::string LogicalTableScan::GetName() { return "LogcialTableScan"; }
uint64_t LogicalTableScan::GetArity() { return 0; }
uint64_t LogicalTableScan::Hash() { return 0; }
uint64_t LogicalTableScan::GetId() { return TableScan::GetId(); }
RelOptCost LogicalTableScan::ComputeCost(RelOptCostFactory* factory) { return TableScan::ComputeCost(factory); }
RelOptNodeType LogicalTableScan::GetType() { return kLogicalTableScan; }

const std::string PhysicalTableScan::GetName() { return "PhyscialTableScan"; }
uint64_t PhysicalTableScan::GetArity() { return 0; }
uint64_t PhysicalTableScan::Hash() { return 0; }
uint64_t PhysicalTableScan::GetId() { return TableScan::GetId(); }
RelOptCost PhysicalTableScan::ComputeCost(RelOptCostFactory* factory) { return TableScan::ComputeCost(factory); }
RelOptNodeType PhysicalTableScan::GetType() { return kPhysicalTableScan; }
}  // namespace plan
}  // namespace amdb