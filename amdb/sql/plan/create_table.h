#pragma once

#include "sql/plan/rel_opt_node.h"
#include "sql/plan/rel_opt_table.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace plan {
class CreateTable : public RelOptNode {
 public:
  RelOptTable *GetTable() const;
  void SetTable(RelOptTable *table);
  plan::Cost FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) override;

 private:
  RelOptTable *table_;
};

class LogicalCreateTable : public CreateTable {
 public:
  LogicalCreateTable() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptNodeType GetType() override;
};

class PhysicalCreateTable : public CreateTable {
 public:
  PhysicalCreateTable() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptNodeType GetType() override;
};
}  // namespace plan
}  // namespace amdb
