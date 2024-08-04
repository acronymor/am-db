#include "sql/plan/create_table.h"

namespace amdb {
namespace plan {
RelOptTable* CreateTable::GetTable() const { return table_; }
void CreateTable::SetTable(RelOptTable* table) { table_ = table; }
plan::Cost CreateTable::FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}

const std::string LogicalCreateTable::GetName() { return "LogicalCreateTable"; }
uint64_t LogicalCreateTable::GetArity() { return 0; }
uint64_t LogicalCreateTable::Hash() { return 0; }
RelOptNodeType LogicalCreateTable::GetType() { return RelOptNodeType::kLogicalCreateTable; }

const std::string PhysicalCreateTable::GetName() { return "PhysicalTableScan"; }
uint64_t PhysicalCreateTable::GetArity() { return 0; }
uint64_t PhysicalCreateTable::Hash() { return 0; }
RelOptNodeType PhysicalCreateTable::GetType() { return RelOptNodeType::kPhysicalCreateTable; }
}  // namespace plan
}  // namespace amdb