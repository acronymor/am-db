#include "sql/planner/plan_node_util.h"

#include "sql/context/statement_context.h"

namespace amdb {
namespace planner {

LogicalNode* ToLogicalNode(StatementContext* stmt_ctx, PhysicalNode* physical) {
  LogicalNode* node = nullptr;
  switch (physical->type) {
    case PhysicalNode::kPhysicalResultSet: {
      PhysicalResultSet* physical_result_set = dynamic_cast<PhysicalResultSet*>(physical);
      node = dynamic_cast<LogicalNode*>(ResultSet(stmt_ctx, physical_result_set));
    }; break;

    case PhysicalNode::kPhysicalTableScan: {
      PhysicalTableScan* physical_table_scan = dynamic_cast<PhysicalTableScan*>(physical);
      node = dynamic_cast<LogicalNode*>(TableScan(stmt_ctx, physical_table_scan));
    }; break;

    case PhysicalNode::kPhysicalFilter: {
      PhysicalFilter* physical_filter = dynamic_cast<PhysicalFilter*>(physical);
      node = dynamic_cast<LogicalNode*>(Filter(stmt_ctx, physical_filter));
    }; break;
  }
  return node;
}

PhysicalNode* ToPhysicalNode(StatementContext* stmt_ctx, LogicalNode* logical) {
  PhysicalNode* node = nullptr;
  switch (logical->type) {
    case LogicalNode::kLogicalResultSet: {
      LogicalResultSet* logical_result_set = dynamic_cast<LogicalResultSet*>(logical);
      node = dynamic_cast<PhysicalNode*>(ResultSet(stmt_ctx, logical_result_set));
    }; break;

    case LogicalNode::kLogicalTableScan: {
      LogicalTableScan* logical_table_scan = dynamic_cast<LogicalTableScan*>(logical);
      node = dynamic_cast<PhysicalNode*>(TableScan(stmt_ctx, logical_table_scan));
    }; break;

    case LogicalNode::kLogicalFilter: {
      LogicalFilter* logical_filter = dynamic_cast<LogicalFilter*>(logical);
      node = dynamic_cast<PhysicalNode*>(Filter(stmt_ctx, logical_filter));
    }; break;
  }
  return node;
}

PhysicalTableScan* TableScan(StatementContext* stmt_ctx, LogicalTableScan* logical) {
  PhysicalTableScan* node = stmt_ctx->arena->CreateObject<PhysicalTableScan>();
  node->table_info = logical->table_info;
  node->primary_ranges = logical->primary_ranges;
  node->primary_filters = logical->primary_filters;
  return node;
}

LogicalTableScan* TableScan(StatementContext* stmt_ctx, PhysicalTableScan* physical) {
  LogicalTableScan* node = stmt_ctx->arena->CreateObject<LogicalTableScan>();
  return node;
}

PhysicalFilter* Filter(StatementContext* stmt_ctx, LogicalFilter* logical) {
  PhysicalFilter* node = stmt_ctx->arena->CreateObject<PhysicalFilter>();
  return node;
}

LogicalFilter* Filter(StatementContext* stmt_ctx, PhysicalFilter* physical) {
  LogicalFilter* node = stmt_ctx->arena->CreateObject<LogicalFilter>();
  return node;
}

PhysicalResultSet* ResultSet(StatementContext* stmt_ctx, LogicalResultSet* logical) {
  PhysicalResultSet* node = stmt_ctx->arena->CreateObject<PhysicalResultSet>();
  return node;
}

LogicalResultSet* ResultSet(StatementContext* stmt_ctx, PhysicalResultSet* physical) {
  LogicalResultSet* node = stmt_ctx->arena->CreateObject<LogicalResultSet>();
  return node;
}
}  // namespace planner
}  // namespace amdb
