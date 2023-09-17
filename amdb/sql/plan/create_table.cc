#include "sql/plan/create_table.h"

namespace amdb {
namespace plan {
RelOptTable *CreateTable::GetTable() const { return table_; }
void CreateTable::SetTable(RelOptTable *table) { table_ = table; }

const std::string LogicalCreateTable::GetName() { return std::string(); }
uint64_t LogicalCreateTable::GetArity() { return 0; }
uint64_t LogicalCreateTable::Hash() { return 0; }
RelOptNodeType LogicalCreateTable::GetType() { return kLogicalCreateTable; }
uint64_t LogicalCreateTable::GetId() { return RelOptNode::GetId(); }
RelOptCost LogicalCreateTable::ComputeCost(RelOptCostFactory *factory) { return RelOptCost(0, 0, 0); }

const std::string PhysicalCreateTable::GetName() { return std::string(); }
uint64_t PhysicalCreateTable::GetArity() { return 0; }
uint64_t PhysicalCreateTable::Hash() { return 0; }
RelOptNodeType PhysicalCreateTable::GetType() { return kPhysicalTableScan; }
RelOptCost PhysicalCreateTable::ComputeCost(RelOptCostFactory *factory) { return RelOptCost(0, 0, 0); }
uint64_t PhysicalCreateTable::GetId() { return RelOptNode::GetId(); }
}  // namespace plan
}  // namespace amdb