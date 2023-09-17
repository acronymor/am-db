#pragma once

#include <iostream>

#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace plan {
class Filter : public RelOptNode {
 public:
  Filter() : RelOptNode() {}
  RelOptCost ComputeCost(RelOptCostFactory *factory) override;
};

class LogicalFilter : public Filter {
 public:
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

  const std::vector<expr::ExprNode *> &GetConditions() const;

 private:
  std::vector<expr::ExprNode *> conditions;
};

class PhysicalFilter : public Filter {
 public:
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

 private:
  std::vector<expr::ExprNode *> conditions;

 public:
  const std::vector<expr::ExprNode *> &GetConditions() const;
  void SetConditions(const std::vector<expr::ExprNode *> &conditions);
};
}  // namespace plan
}  // namespace amdb
