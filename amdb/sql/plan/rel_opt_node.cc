#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace plan {
void RelOptNode::AddInput(RelOptNode *node) { this->inputs_.push_back(node); }
}  // namespace plan
}  // namespace amdb