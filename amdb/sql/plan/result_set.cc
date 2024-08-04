#include "sql/plan/result_set.h"

namespace amdb {
namespace plan {

const std::string LogicalResultSet::GetName() { return "LogicalResultSet"; }
uint64_t LogicalResultSet::GetArity() { return 0; }
uint64_t LogicalResultSet::Hash() { return 0; }
RelOptNodeType LogicalResultSet::GetType() { return RelOptNodeType::kLogicalResultSet; }
plan::Cost LogicalResultSet::FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}

const std::string PhysicalResultSet::GetName() { return "PhysicalResultSet"; }
uint64_t PhysicalResultSet::GetArity() { return 0; }
uint64_t PhysicalResultSet::Hash() { return 0; }
RelOptNodeType PhysicalResultSet::GetType() { return RelOptNodeType::kPhysicalResultSet; }
plan::Cost PhysicalResultSet::FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}
}  // namespace plan
}  // namespace amdb
