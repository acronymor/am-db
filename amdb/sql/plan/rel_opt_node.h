#pragma once

#include <atomic>
#include <iostream>
#include <vector>

#include "sql/context/statement_context.h"
#include "sql/plan/rel_opt_cost.h"

namespace amdb {
namespace plan {
enum RelOptNodeType : uint8_t {
  kLogicalTypeInvalid = 0,
  kLogicalTableScan = 1,
  kLogicalFilter = 2,
  kLogicalResultSet = 3,
  kLogicalCreateDatabase = 4,
  kLogicalCreateTable = 5,
  kLogicalInsert = 6,

  kPhysicalTableScan = 100,
  kPhysicalFilter = 102,
  kPhysicalResultSet = 103,
  kPhysicalCreateDatabase = 104,
  kPhysicalCreateTable = 105,
  kPhysicalInsert = 106,
};

struct RelOptNode {
  RelOptNode() = default;

  virtual uint64_t GetId() { return id_.fetch_add(1); };

  virtual const std::string GetName() = 0;

  void AddInput(RelOptNode* node);

  RelOptNode* GetInput(uint64_t i) {
    std::vector<RelOptNode*> inputs = GetInputs();
    return inputs.at(i);
  }

  std::vector<RelOptNode*> GetInputs() { return std::vector<RelOptNode*>(); }

  virtual uint64_t GetArity() = 0;

  virtual RelOptCost ComputeCost(RelOptCostFactory* factory) = 0;

  virtual uint64_t Hash() = 0;

  virtual RelOptNodeType GetType() = 0;

 private:
  std::atomic<uint64_t> id_{0};
  std::vector<RelOptNode*> inputs_;
};
}  // namespace plan
}  // namespace amdb