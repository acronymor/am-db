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

    case PhysicalNode::kPhysicalCreateDatabase: {
      PhysicalCreateDatabase* physical_create_database = dynamic_cast<PhysicalCreateDatabase*>(physical);
      node = dynamic_cast<LogicalNode*>(CreateDatabase(stmt_ctx, physical_create_database));
    }; break;

    case PhysicalNode::kPhysicalCreateTable: {
      PhysicalCreateTable* physical_create_table = dynamic_cast<PhysicalCreateTable*>(physical);
      node = dynamic_cast<LogicalNode*>(CreateTable(stmt_ctx, physical_create_table));
    }; break;

    case PhysicalNode::kPhysicalInsert: {
      PhysicalInsert* physical_insert = dynamic_cast<PhysicalInsert*>(physical);
      node = dynamic_cast<LogicalNode*>(Insert(stmt_ctx, physical_insert));
    }; break;

    default:
      ERROR("Not Support LogicalNode type {}", physical->type);
      break;
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

    case LogicalNode::kLogicalCreateDatabase: {
      LogicalCreateDatabase* logical_create_database = dynamic_cast<LogicalCreateDatabase*>(logical);
      node = dynamic_cast<PhysicalNode*>(CreateDatabase(stmt_ctx, logical_create_database));
    }; break;

    case LogicalNode::kLogicalCreateTable: {
      LogicalCreateTable* logical_create_table = dynamic_cast<LogicalCreateTable*>(logical);
      node = dynamic_cast<PhysicalNode*>(CreateTable(stmt_ctx, logical_create_table));
    }; break;

    case LogicalNode::kLogicalInsert: {
      LogicalInsert* logical_insert = dynamic_cast<LogicalInsert*>(logical);
      node = dynamic_cast<PhysicalNode*>(Insert(stmt_ctx, logical_insert));
    }; break;

    default:
      ERROR("Not Support LogicalNode type {}", logical->type);
      break;
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

PhysicalCreateDatabase* CreateDatabase(StatementContext* stmt_ctx, LogicalCreateDatabase* logical) {
  PhysicalCreateDatabase* node = stmt_ctx->arena->CreateObject<PhysicalCreateDatabase>();
  node->database_info = logical->database_info;
  return node;
}

LogicalCreateDatabase* CreateDatabase(StatementContext* stmt_ctx, PhysicalCreateDatabase* physical) {
  LogicalCreateDatabase* node = stmt_ctx->arena->CreateObject<LogicalCreateDatabase>();
  node->database_info = physical->database_info;
  return node;
}

static PhysicalCreateTable* CreateTable(StatementContext* stmt_ctx, LogicalCreateTable* logical) {
  PhysicalCreateTable* node = stmt_ctx->arena->CreateObject<PhysicalCreateTable>();
  node->table_info = logical->table_info;
  return node;
}

static LogicalCreateTable* CreateTable(StatementContext* stmt_ctx, PhysicalCreateTable* physical) {
  LogicalCreateTable* node = stmt_ctx->arena->CreateObject<LogicalCreateTable>();
  node->table_info = physical->table_info;
  return node;
}

static PhysicalInsert* Insert(StatementContext* stmt_ctx, LogicalInsert* logical) {
  PhysicalInsert* node = stmt_ctx->arena->CreateObject<PhysicalInsert>();
  node->columns = logical->columns;
  node->expr_nodes = logical->expr_nodes;
  node->table_info = logical->table_info;
  node->row_desc = logical->row_desc;
  return node;
}

static LogicalInsert* Insert(StatementContext* stmt_ctx, PhysicalInsert* physical) {
  LogicalInsert* node = stmt_ctx->arena->CreateObject<LogicalInsert>();
  node->columns = physical->columns;
  node->expr_nodes = physical->expr_nodes;
  node->table_info = physical->table_info;
  node->row_desc = physical->row_desc;
  return node;
}
}  // namespace planner
}  // namespace amdb
