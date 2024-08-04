#include "sql/plan/insert.h"

namespace amdb {
namespace plan {
const std::string LogicalInsert::GetName() { return "LogicalInsert"; }
uint64_t LogicalInsert::GetArity() { return 0; }
uint64_t LogicalInsert::Hash() { return 0; }
RelOptNodeType LogicalInsert::GetType() { return RelOptNodeType::kLogicalInsert; }

const std::string PhysicalInsert::GetName() { return "PhysicalInsert"; }
uint64_t PhysicalInsert::GetArity() { return 0; }
uint64_t PhysicalInsert::Hash() { return 0; }
RelOptNodeType PhysicalInsert::GetType() { return RelOptNodeType::kPhysicalInsert; }
RelOptTable *Insert::GetTable() const { return table_; }
void Insert::SetTable(RelOptTable *table) { table_ = table; }
std::vector<std::vector<expr::ExprNode *>> &Insert::ExprNodes() { return expr_nodes_; }
plan::Cost Insert::FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}
}  // namespace plan
}  // namespace amdb