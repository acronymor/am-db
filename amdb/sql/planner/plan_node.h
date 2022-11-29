#pragma once

#include <iostream>

#include "sql/common/statuscode.h"
#include "sql/expression/expr_node.h"

namespace amdb {
namespace planner {
struct PlanNode {
  enum PlanType : uint8_t {
    kPlanLogical = 0,
    kPlanPhysical = 1,
  };
  PlanNode(PlanType plan_type) : type(plan_type) {}
  virtual ~PlanNode() = default;

  virtual std::string Name() = 0;
  virtual std::string ToString() = 0;

  PlanNode* parent = nullptr;
  PlanType type;
  uint64_t limit = -1;
};
}  // namespace planner
}  // namespace amdb