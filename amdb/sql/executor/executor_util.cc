#include "sql/executor/executor_util.h"

namespace amdb {
namespace executor {
scheduler::IExecutor* ToExecutor(StatementContext* ctx, plan::RelOptNode* physical) {
  scheduler::IExecutor* executor;

  switch (physical->GetType()) {
    case plan::RelOptNodeType::kPhysicalResultSet: {
      plan::PhysicalResultSet* result_set = dynamic_cast<plan::PhysicalResultSet*>(physical);
      executor = ResultSet(ctx, result_set);
    }; break;

    case plan::RelOptNodeType::kPhysicalTableScan: {
      plan::PhysicalTableScan* table_scan = dynamic_cast<plan::PhysicalTableScan*>(physical);
      executor = TableScan(ctx, table_scan);
    }; break;

    case plan::RelOptNodeType::kPhysicalFilter: {
      plan::PhysicalFilter* filter = dynamic_cast<plan::PhysicalFilter*>(physical);
      executor = Filter(ctx, filter);
    }; break;

    case plan::RelOptNodeType::kPhysicalCreateDatabase: {
      plan::PhysicalCreateDatabase* create_database = dynamic_cast<plan::PhysicalCreateDatabase*>(physical);
      executor = CreateDatabase(ctx, create_database);
    }; break;

    case plan::RelOptNodeType::kPhysicalCreateTable: {
      plan::PhysicalCreateTable* create_table = dynamic_cast<plan::PhysicalCreateTable*>(physical);
      executor = CreateTable(ctx, create_table);
    }; break;

    case plan::RelOptNodeType::kPhysicalInsert: {
      plan::PhysicalInsert* insert = dynamic_cast<plan::PhysicalInsert*>(physical);
      executor = Insert(ctx, insert);
    }; break;

    default:
      ERROR("Not support physical node type {}", physical->GetType());
      break;
  }
  return executor;
}

TableScanExecutor* TableScan(StatementContext* ctx, plan::PhysicalTableScan* physical) {
  executor::TableScanExecutor* table_scan_executor =
      ctx->arena->CreateObject<executor::TableScanExecutor>(ctx, physical);
  return table_scan_executor;
}

FilterExecutor* Filter(StatementContext* ctx, plan::PhysicalFilter* physical) {
  executor::FilterExecutor* filter_executor =
      ctx->arena->CreateObject<executor::FilterExecutor>(ctx, physical);
  return filter_executor;
}

ResultSetExecutor* ResultSet(StatementContext* ctx, plan::PhysicalResultSet* physical) {
  executor::ResultSetExecutor* result_set_executor =
      ctx->arena->CreateObject<executor::ResultSetExecutor>(ctx, physical);
  return result_set_executor;
}

CreateDatabaseExecutor* CreateDatabase(StatementContext* ctx, plan::PhysicalCreateDatabase* physical) {
  executor::CreateDatabaseExecutor* create_database_executor =
      ctx->arena->CreateObject<executor::CreateDatabaseExecutor>(ctx, physical);
  return create_database_executor;
}

CreateTableExecutor* CreateTable(StatementContext* ctx, plan::PhysicalCreateTable* physical) {
  executor::CreateTableExecutor* create_table_executor =
      ctx->arena->CreateObject<executor::CreateTableExecutor>(ctx, physical);
  return create_table_executor;
}

InsertExecutor* Insert(StatementContext* ctx, plan::PhysicalInsert* physical) {
  executor::InsertExecutor* insert_executor =
      ctx->arena->CreateObject<executor::InsertExecutor>(ctx, scheduler::ExecutorType::kExecInsert, physical);
  return insert_executor;
}

}  // namespace executor
}  // namespace amdb