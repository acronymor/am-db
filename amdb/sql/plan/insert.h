#pragma once

#include <iostream>

#include "sql/plan/rel_opt_node.h"
#include "sql/plan/rel_opt_table.h"

namespace amdb {
namespace plan {
class Insert : public RelOptNode {
 public:
  Insert() : RelOptNode() {}
  RelOptCost ComputeCost(RelOptCostFactory *factory) override;

  RelOptTable *GetTable() const;
  void SetTable(RelOptTable *table);

  std::vector<std::vector<expr::ExprNode *>> &ExprNodes();

 private:
  RelOptTable *table_;
  std::vector<std::vector<expr::ExprNode *>> expr_nodes_;
};

class LogicalInsert : public Insert {
 public:
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptNodeType GetType() override;
};

class PhysicalInsert : public Insert {
 public:
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;

  RelOptNodeType GetType() override;

 private:
  std::vector<expr::ExprNode *> conditions;
};
}  // namespace plan
}  // namespace amdb
