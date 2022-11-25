#include "sql/executor/filter_executor.h"
#include "sql/executor/resultset_executor.h"
#include "sql/executor/table_scan_executor.h"
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

TEST_F(SchedulerTest, Connect) {
  chunk::Chunk* rows = GenRows(GenRowDesc(), 1026);
  Status status = table_->Insert(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::vector<IExecutor*> executors;

  executor::TableScanExecutor* table_scan_executor =
      arena_->CreateObject<executor::TableScanExecutor>(
          ctx, IExecutor::Type::kExecTableScan, table_->table_info_);
  executor::FilterExecutor* filter_executor =
      arena_->CreateObject<executor::FilterExecutor>(
          ctx, IExecutor::Type::kExecFilter);
  executor::ResultSetExecutor* resultset_executor =
      arena_->CreateObject<executor::ResultSetExecutor>(
          ctx, IExecutor::Type::kExecResultSet);

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
