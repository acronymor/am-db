#include "sql/scheduler/scheduler.h"

#include "gtest/gtest.h"
#include "sql/plan/filter.h"
#include "sql/plan/result_set.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace scheduler {
class SchedulerTest : public testsuite::TableDataGen {};

TEST_F(SchedulerTest, ExecutePlanTest) {
  plan::LogicalResultSet* result_set = ctx->arena->CreateObject<plan::LogicalResultSet>();
  plan::LogicalFilter* filter = ctx->arena->CreateObject<plan::LogicalFilter>();
  plan::LogicalTableScan* table_scan = ctx->arena->CreateObject<plan::LogicalTableScan>();

  plan::IndexRange* ir = ctx->arena->CreateObject<plan::IndexRange>();
  ir->lower.value.push_back(expr::ExprValue::NewString("", ctx->arena));
  ir->lower.type = plan::IndexRange::RangePointType::LEFT_CLOSE;
  ir->lower_str = "";
  ir->upper.value.push_back(expr::ExprValue::NewUInt8(3));
  ir->upper.value.push_back(expr::ExprValue::NewString("c", ctx->arena));
  ir->upper.type = plan::IndexRange::RangePointType::RIGHT_OPEN;
  ir->upper_str = "c";
  table_scan->PrimaryRanges()->push_back(ir);

  ctx->logical_plan = result_set;
  result_set->AddInput(filter);
  filter->AddInput(table_scan);

  // -------------------------------

  // Status status = plan::BuildPlan(ctx);
  // AMDB_ASSERT_EQ(Status::C_OK, status);
  // AMDB_ASSERT_TRUE(ctx->resolved_plan != nullptr);

  // status = scheduler::ExecutePlan(ctx);
  // AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace scheduler
}  // namespace amdb
