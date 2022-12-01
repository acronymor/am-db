#include "sql/planner/physical_plan_node.h"

#include "sql/planner/plan_node_visitor.h"

namespace amdb {
namespace planner {
void PhysicalNode::Accept(PlanNodeVisitor* visitor) { visitor->Visit(this); }

void PhysicalNode::AddChild(PhysicalNode* child) {
  children.push_back(child);
  child->parent = this;
}

std::string PhysicalTableScan::ToString() {
  std::string res;
  res.append("PhysicalTableScan::ToString()");
  return res;
}

std::string PhysicalFilter::ToString() {
  std::string res;
  res.append("PhysicalFilter::ToString()");
  return res;
}

std::string PhysicalResultSet::ToString() {
  std::string res;
  res.append("PhysicalResultSet::ToString()");
  return res;
}
}  // namespace planner
}  // namespace amdb
