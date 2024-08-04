#include "sql/common/statuscode.h"
#include "sql/context/statement_context.h"
#include "sql/plan/create_database.h"
#include "sql/plan/create_table.h"
#include "sql/plan/insert.h"
namespace amdb {
namespace opt {
Status BuildPlan(StatementContext* ctx);

static plan::RelOptNode* ToPhysicalNode(StatementContext* stmt_ctx, plan::RelOptNode* logical);
static plan::PhysicalCreateDatabase* CreateDatabase(StatementContext* stmt_ctx, plan::LogicalCreateDatabase* logical);
static plan::PhysicalCreateTable* CreateTable(StatementContext* stmt_ctx, plan::LogicalCreateTable* logical);
static plan::PhysicalInsert* Insert(StatementContext* stmt_ctx, plan::LogicalInsert* logical);

}  // namespace opt
}  // namespace amdb