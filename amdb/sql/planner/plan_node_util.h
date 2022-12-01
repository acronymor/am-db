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
}  // namespace planner
}  // namespace amdb