#include "sql/plan/create_database.h"

namespace amdb {
namespace plan {
RelOptCost CreateDatabase::ComputeCost(RelOptCostFactory *factory) { return RelOptCost(0, 0, 0); }
RelOptTable *CreateDatabase::GetTable() const { return this->table_; }
void CreateDatabase::SetTable(RelOptTable *table) { this->table_ = table; }
RelOptNodeType LogicalCreateDatabase::GetType() { return kLogicalCreateDatabase; }

const std::string LogicalCreateDatabase::GetName() { return std::string(); }
uint64_t LogicalCreateDatabase::GetArity() { return 0; }
uint64_t LogicalCreateDatabase::Hash() { return 0; }
RelOptNodeType PhysicalCreateDatabase::GetType() { return kPhysicalCreateDatabase; }

const std::string PhysicalCreateDatabase::GetName() { return std::string(); }
uint64_t PhysicalCreateDatabase::GetArity() { return 0; }
uint64_t PhysicalCreateDatabase::Hash() { return 0; }
};  // namespace plan
};  // namespace amdb