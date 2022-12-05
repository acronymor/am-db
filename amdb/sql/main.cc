#include "sql/main.h"

#include "common/log.h"
#include "common/version.h"
#include "gflags/gflags.h"
#include "sql/analyzer/analyzer.h"
#include "sql/parser/parser.h"
#include "sql/planner/planner.h"
#include "sql/scheduler/scheduler.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {

int init(int argc, char* argv[]) {
  // init gflags
  gflags::SetVersionString(GetRcsid());
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // init log
  if (init_log(argv[0]) != 0) {
    FATAL("{}", "log init failed.")
    return -1;
  }

  // init storage
  storage::StorageAPIOptions options;
  Status status = storage::KvStorageAPISingleton::Init(options);
  if (Status::C_OK != status) {
    FATAL("{}", GetErrorString(status));
    return -1;
  }

  return 0;
}

chunk::RowDescriptor* GenTestDesc(Arena* arena) {
  chunk::ColumnDescriptor* col_desc_1 = arena->CreateObject<chunk::ColumnDescriptor>(expr::ExprValueType::UInt64, 0);
  chunk::ColumnDescriptor* col_desc_2 = arena->CreateObject<chunk::ColumnDescriptor>(expr::ExprValueType::String, 1);
  chunk::ColumnDescriptor* col_desc_3 = arena->CreateObject<chunk::ColumnDescriptor>(expr::ExprValueType::UInt8, 2);

  chunk::RowDescriptor* row_desc = arena->CreateObject<chunk::RowDescriptor>(0);
  row_desc->AddColumnDesc(col_desc_1);
  row_desc->AddColumnDesc(col_desc_2);
  row_desc->AddColumnDesc(col_desc_3);
  row_desc->InitAllColDesc();

  return row_desc;
}

Status run() {
  MemTracker* tracker = new MemTracker();
  StatementContext* stmt_ctx = new StatementContext();
  stmt_ctx->arena = new Arena(tracker);
  stmt_ctx->raw_sql = "SELECT * from t";
  stmt_ctx->row_desc = GenTestDesc(stmt_ctx->arena);

  Status status = Status::C_OK;

  status = parser::GenAst(stmt_ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = analyzer::AnalyzeAst(stmt_ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = planner::BuildPlan(stmt_ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = scheduler::ExecutePlan(stmt_ctx);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  return Status::C_OK;
}
}  // namespace amdb

int main(int argc, char* argv[]) {
  amdb::init(argc, argv);
  amdb::run();
}