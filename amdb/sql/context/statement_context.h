#pragma once

#include "sql/chunk/row_description.h"
#include "sql/common/arena.h"
#include "sql/planner/logical_plan_node.h"
#include "sql/planner/physical_plan_node.h"

namespace amdb {
class StatementContext {
 public:
  std::string raw_sql;
  Arena* arena;

  chunk::RowDescriptor* row_desc;

  // it is either logical_plan or physical_plan
  planner::PlanNode* resolved_plan{nullptr};
  planner::LogicalNode* logical_plan{nullptr};
  planner::PhysicalNode* physical_plan{nullptr};
  // parser::StmtNode* ast_stmt = nullptr;
};
}  // namespace amdb