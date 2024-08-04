#pragma once

#include "sql/plan/rel_opt_node.h"
#include "sql/plan/rel_opt_table.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace plan {
class CreateDatabase : public RelOptNode {
 public:
  CreateDatabase() = default;
  plan::Cost FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) override;

  RelOptTable* GetTable() const;
  void SetTable(RelOptTable* table);

 private:
  RelOptTable* table_;
};

class LogicalCreateDatabase : public CreateDatabase {
 public:
  LogicalCreateDatabase() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptNodeType GetType() override;
};

class PhysicalCreateDatabase : public CreateDatabase {
 public:
  PhysicalCreateDatabase() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptNodeType GetType() override;
};
}  // namespace plan
}  // namespace amdb
