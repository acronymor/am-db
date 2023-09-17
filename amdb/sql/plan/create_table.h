#pragma once

#include "sql/plan/rel_opt_node.h"
#include "sql/plan/rel_opt_table.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace plan {
class CreateTable : public RelOptNode {
 public:
  CreateTable() : RelOptNode() {}

  RelOptTable *GetTable() const;
  void SetTable(RelOptTable *table);

 private:
  RelOptTable *table_;
};

class LogicalCreateTable : public CreateTable {
 public:
  const std::string GetName() override;
  uint64_t GetId() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptCost ComputeCost(RelOptCostFactory *factory) override;

  RelOptNodeType GetType() override;
};

class PhysicalCreateTable : public CreateTable {
 public:
  uint64_t GetId() override;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptNodeType GetType() override;
  RelOptCost ComputeCost(RelOptCostFactory *factory) override;


};
}  // namespace plan
}  // namespace amdb
