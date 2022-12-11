#include "sql/planner/planner.h"

#include "sql/analyzer/analyzer.h"
#include "sql/parser/parser.h"
#include "sql/storage/table.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace planner {
struct PlannerTest : public testsuite::SchemaGen {
 protected:
  void SetUp() override {
    testsuite::SchemaGen::SetUp();
    uint64_t db_id = 0;
    uint64_t table_id = 0;
    schema::TableInfo* table_info = GenTableInfo(db_id, table_id);
    table_ = arena_->CreateObject<storage::Table>(arena_, table_info);
    ctx = new StatementContext();
    ctx->arena = arena_;
    ctx->row_desc = GenRowDesc();
    ctx->row_desc->InitAllColDesc();
  }

  void TearDown() override {
    SchemaGen::TearDown();
    delete ctx;
  }

 protected:
  storage::Table* table_;
  StatementContext* ctx;
};

TEST_F(PlannerTest, BuildPlanTest) {
  schema::TableInfo* table_info = table_->table_info_;
  storage::Metadata meta(instance_);

  Status status = Status::C_OK;
  status = meta.DumpTableMeta(table_info->db_id, table_info->id, table_info);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  ctx->raw_sql = "SELECT * FROM test_db.test_table";

  status = parser::GenAst(ctx);
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
