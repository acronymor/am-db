#include "sql/planner/logical_plan_node.h"

#include "sql/planner/plan_node_visitor.h"

namespace amdb {
namespace planner {

void LogicalNode::Accept(PlanNodeVisitor* visitor) { visitor->Visit(this); }

void LogicalNode::AddChild(LogicalNode* child) {
  children.push_back(child);
  child->parent = this;
}

std::string LogicalTableScan::ToString() {
  std::string res;
  res.append("LogicalTableScan::ToString()");
  return res;
}

std::string LogicalFilter::ToString() {
  std::string res;
  res.append("LogicalFilter::ToString()");
  return res;
}

std::string LogicalResultSet::ToString() {
  std::string res;
  res.append("LogicalResultSet::ToString()");
  return res;
}

std::string LogicalCreateDatabase::ToString() {
  std::string res;
  res.append("LogicalCreateDatabase::ToString()");
  return res;
}

std::string LogicalCreateTable::ToString() {
  std::string res;
  res.append("LogicalCreateTable::ToString()");
  return res;
}
}  // namespace planner
}  // namespace amdb
