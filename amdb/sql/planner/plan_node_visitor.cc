#include "sql/planner/plan_node_visitor.h"

namespace amdb {
namespace planner {
void LogicalVisitor::Visit(LogicalNode* root) {
  for (LogicalNode* node : root->children) {
    std::cout << "LogicalVisitor::" << node->Name() << std::endl;
    switch (node->type) {
      case LogicalNode::kLogicalTableScan:
        break;
      case LogicalNode::kLogicalFilter:
        break;
    }
    node->Accept(this);
  }
}

void PhysicalVisitor::Visit(PhysicalNode* root) {
  for (PhysicalNode* node : root->children) {
    std::cout << "PhysicalVisitor::" << node->Name() << std::endl;
    switch (node->type) {
      case PhysicalNode::kPhysicalTableScan:
        break;
      case PhysicalNode::kPhysicalFilter:
        break;
    }
    node->Accept(this);
  }
}
}  // namespace planner
}  // namespace amdb
