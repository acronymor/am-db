#pragma once

#include <iostream>
#include <vector>

#include "sql/chunk/row_description.h"
#include "sql/common/arena.h"

namespace amdb {
namespace expr {
class ExprNode;
}  // namespace expr

namespace planner {
struct PlanNode;
struct LogicalNode;
struct PhysicalNode;
}  // namespace planner

class StatementContext {
 public:
  std::string raw_sql;
  Arena* arena;

  chunk::RowDescriptor* row_desc;

  // parser::StmtNode* ast_stmt = nullptr;
  // it is either logical_plan or physical_plan
  planner::PlanNode* resolved_plan{nullptr};
  planner::LogicalNode* logical_plan{nullptr};
  planner::PhysicalNode* physical_plan{nullptr};
};
}  // namespace amdb