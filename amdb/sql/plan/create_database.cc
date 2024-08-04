#include "sql/plan/create_database.h"

namespace amdb {
namespace plan {
plan::Cost CreateDatabase::FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) {
  return plan::Cost(0.0, 0.0, 0.0, 0.0);
}
RelOptTable* CreateDatabase::GetTable() const { return this->table_; }
void CreateDatabase::SetTable(RelOptTable* table) { this->table_ = table; }
RelOptNodeType LogicalCreateDatabase::GetType() { return RelOptNodeType::kLogicalCreateDatabase; }

const std::string LogicalCreateDatabase::GetName() { return "LogicalCreateDatabase"; }
uint64_t LogicalCreateDatabase::GetArity() { return 0; }
uint64_t LogicalCreateDatabase::Hash() { return 0; }
RelOptNodeType PhysicalCreateDatabase::GetType() { return RelOptNodeType::kPhysicalCreateDatabase; }

const std::string PhysicalCreateDatabase::GetName() { return "PhysicalCreateDatabase"; }
uint64_t PhysicalCreateDatabase::GetArity() { return 0; }
uint64_t PhysicalCreateDatabase::Hash() { return 0; }
};  // namespace plan
};  // namespace amdb