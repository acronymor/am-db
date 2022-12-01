#pragma once

#include "sql/planner/logical_plan_node.h"
#include "sql/planner/physical_plan_node.h"

namespace amdb {
namespace planner {
struct PlanNodeVisitor {
  ~PlanNodeVisitor() = default;

  virtual void Visit(LogicalNode* node) = 0;

  virtual void Visit(PhysicalNode* node) = 0;
};

struct LogicalVisitor : public PlanNodeVisitor {
  void Visit(LogicalNode* node) override;
  void Visit(PhysicalNode* node) override{};
};

struct PhysicalVisitor : public PlanNodeVisitor {
  void Visit(LogicalNode* node) override{};
  void Visit(PhysicalNode* node) override;
};
}  // namespace planner
}  // namespace amdb