#pragma once

#include <iostream>
#include <vector>

#include "sql/common/arena.h"
#include "sql/parser/base.h"
#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace expr {
class ExprNode;
}  // namespace expr

namespace plan {
struct RelOptNode;
}  // namespace planner

class StatementContext {
 public:
  std::string raw_sql;
  Arena* arena;

  parser::Node* stmt_ast{nullptr};
  plan::RelOptNode* resolved_plan{nullptr};
  plan::RelOptNode* logical_plan{nullptr};
  plan::RelOptNode* physical_plan{nullptr};
};
}  // namespace amdb