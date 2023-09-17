#pragma once

#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace plan {
class ResultSet : public RelOptNode {
 public:
  ResultSet() : RelOptNode() {}
};

class LogicalResultSet : public ResultSet {
 public:
  LogicalResultSet() : ResultSet(){};
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

  RelOptCost ComputeCost(RelOptCostFactory *factory) override;
};

class PhysicalResultSet : public ResultSet {
 public:
  PhysicalResultSet() : ResultSet(){};
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

  RelOptCost ComputeCost(RelOptCostFactory *factory) override;
};
};  // namespace plan
}  // namespace amdb
