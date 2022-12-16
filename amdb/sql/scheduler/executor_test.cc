#include "sql/executor/filter_executor.h"
#include "sql/executor/resultset_executor.h"
#include "sql/executor/table_scan_executor.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace scheduler {
struct ExecutorTest : public testsuite::TableDataGen {};

TEST_F(ExecutorTest, Connect) {
  std::vector<IExecutor*> executors;

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

  executors.reserve(3);
  executors.emplace_back(table_scan_executor);
  executors.emplace_back(filter_executor);
  executors.emplace_back(resultset_executor);

  // Connect
  for (size_t i = 0; i < executors.size() - 1; i++) {
    Connect(executors.at(i)->CreateOutputPort(), executors.at(i + 1)->CreateInputPort());
  }

  // Open
  for (size_t i = 0; i < executors.size(); i++) {
    Status status = executors.at(i)->Open();
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  // Prepare
  for (size_t i = 0; i < executors.size(); i++) {
    executors.at(i)->Prepare();
  }

  // Work
  for (size_t i = 0; i < executors.size(); i++) {
    Status status = executors.at(i)->Work();
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  // Close
  for (size_t i = 0; i < executors.size(); i++) {
    Status status = executors.at(i)->Close();
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }
}
}  // namespace scheduler
}  // namespace amdb
