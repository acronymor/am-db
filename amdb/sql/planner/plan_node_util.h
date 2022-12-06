#include "sql/planner/logical_plan_node.h"
#include "sql/planner/physical_plan_node.h"

namespace amdb {
namespace planner {
PhysicalNode* ToPhysicalNode(StatementContext* stmt_ctx, LogicalNode* logical);
LogicalNode* ToLogicalNode(StatementContext* stmt_ctx, PhysicalNode* physical);

static PhysicalTableScan* TableScan(StatementContext* stmt_ctx, LogicalTableScan* logical);
static LogicalTableScan* TableScan(StatementContext* stmt_ctx, PhysicalTableScan* physical);

static PhysicalFilter* Filter(StatementContext* stmt_ctx, LogicalFilter* logical);
static LogicalFilter* Filter(StatementContext* stmt_ctx, PhysicalFilter* physical);

static PhysicalResultSet* ResultSet(StatementContext* stmt_ctx, LogicalResultSet* logical);
static LogicalResultSet* ResultSet(StatementContext* stmt_ctx, PhysicalResultSet* physical);

static PhysicalCreateDatabase* CreateDatabase(StatementContext* stmt_ctx, LogicalCreateDatabase* logical);
static LogicalCreateDatabase* CreateDatabase(StatementContext* stmt_ctx, PhysicalCreateDatabase* physical);

static PhysicalCreateTable* CreateTable(StatementContext* stmt_ctx, LogicalCreateTable* logical);
static LogicalCreateTable* CreateTable(StatementContext* stmt_ctx, PhysicalCreateTable* physical);
}  // namespace planner
}  // namespace amdb