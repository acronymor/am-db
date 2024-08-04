#include "sql/analyzer/analyzer.h"

#include "gtest/gtest.h"
#include "sql/parser/parser.h"
#include "sql/plan/rel_opt_node.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace analyzer {
class AnalyzerTest : public testsuite::TableDataGen {};

TEST_F(AnalyzerTest, TestAnalyze) {
  ctx->raw_sql = "SELECT * FROM test_db_0.test_table_0";

  Status status = parser::GenAst(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = analyzer::AnalyzeAst(ctx);
  AMDB_ASSERT_TRUE(ctx->logical_plan != nullptr);
  AMDB_ASSERT_EQ(plan::RelOptNodeType::kLogicalResultSet, ctx->logical_plan->GetType());
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace analyzer
}  // namespace amdb