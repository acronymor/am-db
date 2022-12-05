#include "sql/analyzer/analyzer.h"

#include "sql/parser/parser.h"
#include "sql/planner/logical_plan_node.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace analyzer {
class AnalyzerTest : public testsuite::SchemaGen {
 protected:
  void SetUp() override {
    testsuite::SchemaGen::SetUp();

    ctx = new StatementContext();
    ctx->arena = arena_;
  }

  void TearDown() override {
    delete ctx;

    testsuite::SchemaGen::TearDown();
  }

 protected:
  StatementContext* ctx;
};

TEST_F(AnalyzerTest, TestAnalyze) {
  ctx->raw_sql = "SELECT * FROM t";

  Status status = parser::GenAst(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = analyzer::AnalyzeAst(ctx);
  AMDB_ASSERT_TRUE(ctx->logical_plan != nullptr);
  AMDB_ASSERT_EQ(planner::LogicalNode::kLogicalResultSet, ctx->logical_plan->type);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace analyzer
}  // namespace amdb