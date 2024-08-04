#pragma once

#include <iostream>

#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace plan {
class Filter : public RelOptNode {};

class LogicalFilter : public Filter {
 public:
  LogicalFilter() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

  const std::vector<expr::ExprNode *> &GetConditions() const;
  plan::Cost FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) override;

 private:
  std::vector<expr::ExprNode *> conditions;
};

class PhysicalFilter : public Filter {
 public:
  PhysicalFilter() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;
  plan::Cost FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) override;

 private:
  std::vector<expr::ExprNode *> conditions;

 public:
  const std::vector<expr::ExprNode *> &GetConditions() const;
  void SetConditions(const std::vector<expr::ExprNode *> &conditions);
};
}  // namespace plan
}  // namespace amdb
