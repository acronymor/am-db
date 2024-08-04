#pragma once

#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace plan {
class ResultSet : public RelOptNode {};

class LogicalResultSet : public ResultSet {
 public:
  LogicalResultSet() = default;

  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

  plan::Cost FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) override;
};

class PhysicalResultSet : public ResultSet {
 public:
  PhysicalResultSet() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

  plan::Cost FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) override;
};
};  // namespace plan
}  // namespace amdb
