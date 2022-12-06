#include "sql/executor/executor_util.h"

namespace amdb {
namespace executor {
scheduler::IExecutor* ToExecutor(StatementContext* ctx, planner::PhysicalNode* physical) {
  scheduler::IExecutor* executor;

  switch (physical->type) {
    case planner::PhysicalNode::kPhysicalResultSet: {
      planner::PhysicalResultSet* result_set = dynamic_cast<planner::PhysicalResultSet*>(physical);
      executor = ResultSet(ctx, result_set);
    }; break;
    case planner::PhysicalNode::kPhysicalTableScan: {
      planner::PhysicalTableScan* table_scan = dynamic_cast<planner::PhysicalTableScan*>(physical);
      executor = TableScan(ctx, table_scan);
    }; break;
    case planner::PhysicalNode::kPhysicalFilter: {
      planner::PhysicalFilter* filter = dynamic_cast<planner::PhysicalFilter*>(physical);
      executor = Filter(ctx, filter);
    }; break;
    case planner::PhysicalNode::kPhysicalCreateDatabase: {
      planner::PhysicalCreateDatabase* create_database = dynamic_cast<planner::PhysicalCreateDatabase*>(physical);
      executor = CreateDatabase(ctx, create_database);
    }; break;
    case planner::PhysicalNode::kPhysicalCreateTable: {
      planner::PhysicalCreateTable* create_table = dynamic_cast<planner::PhysicalCreateTable*>(physical);
      executor = CreateTable(ctx, create_table);
    }; break;

    default:
      ERROR("Not support physical node type {}", physical->type);
      break;
  }
  return executor;
}

TableScanExecutor* TableScan(StatementContext* ctx, planner::PhysicalTableScan* physical) {
  executor::TableScanExecutor* table_scan_executor =
      ctx->arena->CreateObject<executor::TableScanExecutor>(ctx, scheduler::IExecutor::kExecTableScan, physical);
  return table_scan_executor;
}

FilterExecutor* Filter(StatementContext* ctx, planner::PhysicalFilter* physical) {
  executor::FilterExecutor* filter_executor =
      ctx->arena->CreateObject<executor::FilterExecutor>(ctx, scheduler::IExecutor::kExecFilter, physical);
  return filter_executor;
}

ResultSetExecutor* ResultSet(StatementContext* ctx, planner::PhysicalResultSet* physical) {
  executor::ResultSetExecutor* result_set_executor =
      ctx->arena->CreateObject<executor::ResultSetExecutor>(ctx, scheduler::IExecutor::kExecResultSet, physical);
  return result_set_executor;
}

CreateDatabaseExecutor* CreateDatabase(StatementContext* ctx, planner::PhysicalCreateDatabase* physical) {
  executor::CreateDatabaseExecutor* create_database_executor =
      ctx->arena->CreateObject<executor::CreateDatabaseExecutor>(ctx, scheduler::IExecutor::kExecCreateDatabase,
                                                                 physical);
  return create_database_executor;
}

CreateTableExecutor* CreateTable(StatementContext* ctx, planner::PhysicalCreateTable* physical) {
  executor::CreateTableExecutor* create_table_executor =
      ctx->arena->CreateObject<executor::CreateTableExecutor>(ctx, scheduler::IExecutor::kExecCreateTable, physical);
  return create_table_executor;
}
}  // namespace executor
}  // namespace amdb