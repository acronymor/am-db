#include "sql/scheduler/scheduler.h"

#include "gtest/gtest.h"
#include "sql/planner/planner.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace scheduler {
class SchedulerTest : public testsuite::TableDataGen {};

TEST_F(SchedulerTest, ExecutePlanTest) {
  planner::LogicalResultSet* result_set = ctx->arena->CreateObject<planner::LogicalResultSet>();
  planner::LogicalFilter* filter = ctx->arena->CreateObject<planner::LogicalFilter>();
  planner::LogicalTableScan* table_scan = ctx->arena->CreateObject<planner::LogicalTableScan>();

  planner::IndexRange* ir = ctx->arena->CreateObject<planner::IndexRange>();
  ir->lower.value.push_back(expr::ExprValue::NewString("", ctx->arena));
  ir->lower.type = planner::IndexRange::RangePointType::LEFT_CLOSE;
  ir->lower_str = "";
  ir->upper.value.push_back(expr::ExprValue::NewUInt8(3));
  ir->upper.value.push_back(expr::ExprValue::NewString("c", ctx->arena));
  ir->upper.type = planner::IndexRange::RangePointType::RIGHT_OPEN;
  ir->upper_str = "c";
  table_scan->primary_ranges.push_back(ir);
  table_scan->table_info = table_->table_info_;

  ctx->logical_plan = result_set;
  result_set->AddChild(filter);
  filter->AddChild(table_scan);

  // -------------------------------

  Status status = planner::BuildPlan(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_TRUE(ctx->resolved_plan != nullptr);

  status = scheduler::ExecutePlan(ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace scheduler
}  // namespace amdb
