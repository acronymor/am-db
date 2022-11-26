#include "sql/scheduler/scheduler.h"

#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace scheduler {
struct SchedulerTest : public testsuite::SchemaGen {
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

TEST_F(SchedulerTest, ExecutePlanTest) {
  chunk::Chunk* rows = GenRows(GenRowDesc(), 10);
  Status status = table_->Insert(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  Scheduler scheduler(ctx);
  status = scheduler.TestRun(table_->table_info_);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace scheduler
}  // namespace amdb
