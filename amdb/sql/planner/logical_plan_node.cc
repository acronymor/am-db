#include "sql/planner/logical_plan_node.h"

namespace amdb {
namespace planner {

void LogicalNode::AddChild(LogicalNode *child) {
  children.push_back(child);
  child->parent = this;
}
}
}
