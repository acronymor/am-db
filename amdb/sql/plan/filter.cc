#include "sql/plan/filter.h"

namespace amdb {
namespace plan {
RelOptCost Filter::ComputeCost(RelOptCostFactory *factory) { return RelOptCost(0, 0, 0); }

const std::string LogicalFilter::GetName() { return "LogicalFilter"; }
uint64_t LogicalFilter::GetArity() { return 0; }
uint64_t LogicalFilter::Hash() { return 0; }
RelOptNodeType LogicalFilter::GetType() { return kLogicalFilter; }
const std::vector<expr::ExprNode *> &LogicalFilter::GetConditions() const { return conditions; }

const std::string PhysicalFilter::GetName() { return "PhysicalFilter"; }
uint64_t PhysicalFilter::GetArity() { return 0; }
uint64_t PhysicalFilter::Hash() { return 0; }
RelOptNodeType PhysicalFilter::GetType() { return kPhysicalFilter; }
const std::vector<expr::ExprNode *> &PhysicalFilter::GetConditions() const { return conditions; }
void PhysicalFilter::SetConditions(const std::vector<expr::ExprNode *> &conditions) {
  PhysicalFilter::conditions = conditions;
}
}  // namespace plan
}  // namespace amdb