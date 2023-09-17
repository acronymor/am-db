#include "sql/executor/create_executor.h"
#include "sql/executor/filter_executor.h"
#include "sql/executor/insert_executor.h"
#include "sql/executor/resultset_executor.h"
#include "sql/executor/table_scan_executor.h"
#include "sql/plan/create_database.h"
#include "sql/plan/create_table.h"
#include "sql/plan/filter.h"
#include "sql/plan/insert.h"
#include "sql/plan/result_set.h"

namespace amdb {
namespace executor {
scheduler::IExecutor* ToExecutor(StatementContext* stmt_ctx, plan::RelOptNode* physical);

static TableScanExecutor* TableScan(StatementContext* stmt_ctx, plan::PhysicalTableScan* physical);

static FilterExecutor* Filter(StatementContext* stmt_ctx, plan::PhysicalFilter* physical);

static ResultSetExecutor* ResultSet(StatementContext* stmt_ctx, plan::PhysicalResultSet* physical);

static CreateDatabaseExecutor* CreateDatabase(StatementContext* stmt_ctx, plan::PhysicalCreateDatabase* physical);

static CreateTableExecutor* CreateTable(StatementContext* stmt_ctx, plan::PhysicalCreateTable* physical);

static InsertExecutor* Insert(StatementContext* stmt_ctx, plan::PhysicalInsert* physical);
}  // namespace executor
}  // namespace amdb