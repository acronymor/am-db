#include "sql/optimizer/rules/predicate_pushdown.h"

namespace amdb {
namespace opt {
Status PredicatePushdownRule::Rewrite(planner::LogicalNode* root,
                                      bool* applied) {
  return Status::C_OK;
};

Status PredicatePushdownRule::Rewrite(planner::PhysicalNode* root,
                                      bool* applied) {
  return Status::C_OK;
};
}  // namespace opt
}  // namespace amdb