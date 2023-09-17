#include "sql/plan/result_set.h"

namespace amdb {
namespace plan {

const std::string LogicalResultSet::GetName() { return "LogicalResultSet"; }
uint64_t LogicalResultSet::GetArity() { return 0; }
uint64_t LogicalResultSet::Hash() { return 0; }
RelOptNodeType LogicalResultSet::GetType() { return kLogicalResultSet; }
RelOptCost LogicalResultSet::ComputeCost(RelOptCostFactory *factory) { return RelOptCost(0, 0, 0); }

const std::string PhysicalResultSet::GetName() { return std::string(); }
uint64_t PhysicalResultSet::GetArity() { return 0; }
uint64_t PhysicalResultSet::Hash() { return 0; }
RelOptNodeType PhysicalResultSet::GetType() { return kLogicalResultSet; }
RelOptCost PhysicalResultSet::ComputeCost(RelOptCostFactory *factory) { return RelOptCost(0, 0, 0); }
}  // namespace plan
}  // namespace amdb
