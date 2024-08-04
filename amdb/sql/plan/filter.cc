#include "sql/plan/filter.h"

namespace amdb {
namespace plan {
const std::string LogicalFilter::GetName() { return "LogicalFilter"; }
uint64_t LogicalFilter::GetArity() { return 0; }
uint64_t LogicalFilter::Hash() { return 0; }
RelOptNodeType LogicalFilter::GetType() { return RelOptNodeType::kLogicalFilter; }
const std::vector<expr::ExprNode *> &LogicalFilter::GetConditions() const { return conditions; }
plan::Cost LogicalFilter::FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}

const std::string PhysicalFilter::GetName() { return "PhysicalFilter"; }
uint64_t PhysicalFilter::GetArity() { return 0; }
uint64_t PhysicalFilter::Hash() { return 0; }
RelOptNodeType PhysicalFilter::GetType() { return RelOptNodeType::kPhysicalFilter; }
const std::vector<expr::ExprNode *> &PhysicalFilter::GetConditions() const { return conditions; }
void PhysicalFilter::SetConditions(const std::vector<expr::ExprNode *> &conditions) {
  PhysicalFilter::conditions = conditions;
}
plan::Cost PhysicalFilter::FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}
}  // namespace plan
}  // namespace amdb