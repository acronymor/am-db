#include "sql/scheduler/scheduler.h"

#include "sql/planner/planner.h"
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

TEST_F(SchedulerTest, MockPlanTest) {
  chunk::Chunk* rows = GenRows(GenRowDesc(), 10);
  Status status = table_->Insert(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  planner::IndexRange* ir = ctx->arena->CreateObject<planner::IndexRange>();
  ir->lower.value.push_back(expr::ExprValue::NewString("", ctx->arena));
  ir->lower.type = planner::IndexRange::RangePointType::LEFT_CLOSE;
  ir->lower_str = "";
  ir->upper.value.push_back(expr::ExprValue::NewUInt8(3));
  ir->upper.value.push_back(expr::ExprValue::NewString("c", ctx->arena));
  ir->upper.type = planner::IndexRange::RangePointType::RIGHT_OPEN;
  ir->upper_str = "c";
  planner::PhysicalTableScan* scan_plan = ctx->arena->CreateObject<planner::PhysicalTableScan>();
  scan_plan->table_info = table_->table_info_;
  scan_plan->primary_ranges.push_back(ir);
  executor::TableScanExecutor* table_scan_executor =
      ctx->arena->CreateObject<executor::TableScanExecutor>(ctx, IExecutor::Type::kExecTableScan, scan_plan);

  planner::PhysicalFilter* filter_plan = ctx->arena->CreateObject<planner::PhysicalFilter>();
  executor::FilterExecutor* filter_executor =
      ctx->arena->CreateObject<executor::FilterExecutor>(ctx, IExecutor::Type::kExecFilter, filter_plan);

  planner::PhysicalResultSet* result_plan = ctx->arena->CreateObject<planner::PhysicalResultSet>();
  executor::ResultSetExecutor* resultset_executor =
      ctx->arena->CreateObject<executor::ResultSetExecutor>(ctx, IExecutor::Type::kExecResultSet, result_plan);

  std::vector<IExecutor*> executors;
  executors.reserve(3);
  executors.emplace_back(table_scan_executor);
  executors.emplace_back(filter_executor);
  executors.emplace_back(resultset_executor);

  Scheduler scheduler(ctx);
  status = scheduler.TestRun(&executors);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}

TEST_F(SchedulerTest, ExecutePlanTest) {
  chunk::Chunk* rows = GenRows(GenRowDesc(), 10);
  Status status = table_->Insert(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  schema::TableInfo* table_info = table_->table_info_;
  storage::Metadata meta(instance_);
  meta.DumpTableMeta(table_info->db_id, table_info->id, table_info);

  status = planner::BuildPlan(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_TRUE(ctx->resolved_plan != nullptr);

  status = scheduler::ExecutePlan(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace scheduler
}  // namespace amdb
