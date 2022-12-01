#include "sql/executor/resultset_executor.h"
#include "sql/executor/filter_executor.h"
#include "sql/executor/table_scan_executor.h"
#include "sql/planner/physical_plan_node.h"

namespace amdb {
namespace executor {
scheduler::IExecutor* ToExecutor(StatementContext* stmt_ctx, planner::PhysicalNode* physical);

static TableScanExecutor* TableScan(StatementContext* stmt_ctx, planner::PhysicalTableScan* physical);

static FilterExecutor* Filter(StatementContext* stmt_ctx, planner::PhysicalFilter* physical);

static ResultSetExecutor* ResultSet(StatementContext* stmt_ctx, planner::PhysicalResultSet* physical);
}
}