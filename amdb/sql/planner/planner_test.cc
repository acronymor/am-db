#include "sql/planner/planner.h"

#include "sql/analyzer/analyzer.h"
#include "sql/parser/parser.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace planner {
struct PlannerTest : public testsuite::TableDataGen {};

TEST_F(PlannerTest, BuildPlanTest) {
  ctx->raw_sql = "SELECT * FROM test_db_0.test_table_0";

  Status status = parser::GenAst(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = analyzer::AnalyzeAst(ctx);
  AMDB_ASSERT_TRUE(ctx->logical_plan != nullptr);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = planner::BuildPlan(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_TRUE(ctx->resolved_plan != nullptr);
}
}  // namespace planner
}  // namespace amdb
