#include "common/log.h"
#include "common/version.h"
#include "gflags/gflags.h"
#include "sql/analyzer/analyzer.h"
#include "sql/optimizer/planner.h"
#include "sql/parser/parser.h"
#include "sql/scheduler/scheduler.h"
#include "sql/storage/id_allocator.h"
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
  storage::StorageAPIOptions storage_options;
  Status status = storage::KvStorageAPISingleton::Init(storage_options);
  if (Status::C_OK != status) {
    FATAL("{}", GetErrorString(status));
    return -1;
  }

  // init id allocator
  storage::IdAllocatorApiOptions id_options;
  status = storage::IdAllocatorSingleton::Init(id_options);
  if (Status::C_OK != status) {
    FATAL("{}", GetErrorString(status));
    return -1;
  }

  return 0;
}

Status run(const std::string& sql) {
  MemTracker* tracker = new MemTracker();
  StatementContext* stmt_ctx = new StatementContext();

  stmt_ctx->arena = new Arena(tracker);
  stmt_ctx->raw_sql = sql;

  Status status = Status::C_OK;

  INFO("sql = {}", stmt_ctx->raw_sql);
  status = parser::GenAst(stmt_ctx);
  if (Status::C_OK != status) {
    ERROR("{} {}", status, GetErrorString(status));
    return status;
  }

  status = analyzer::AnalyzeAst(stmt_ctx);
  if (Status::C_OK != status) {
    ERROR("{}", GetErrorString(status));
    return status;
  }

  status = opt::BuildPlan(stmt_ctx);
  if (Status::C_OK != status) {
    ERROR("{}", GetErrorString(status));
    return status;
  }

  status = scheduler::ExecutePlan(stmt_ctx);
  if (Status::C_OK != status) {
    ERROR("{}", GetErrorString(status))
    return status;
  }

  return Status::C_OK;
}
}  // namespace amdb

int main(int argc, char* argv[]) {
  amdb::init(argc, argv);

  amdb::Status status = amdb::Status::C_OK;

  std::string sql1 = "CREATE DATABASE IF NOT EXISTS db_test";
  status = amdb::run(sql1);
  AMDB_ASSERT_EQ(amdb::Status::C_OK, status);

  std::string sql2 = "CREATE TABLE IF NOT EXISTS db_test.db_table(id BIGINT not null, age BIGINT, PRIMARY KEY (`id`))";
  status = amdb::run(sql2);
  AMDB_ASSERT_EQ(amdb::Status::C_OK, status);

  for (int i = 0; i < 10; i++) {
    std::string sql3 =
        "INSERT INTO db_test.db_table(`id`, `age`) VALUES(" + std::to_string(i) + ", " + std::to_string(100 - i) + ")";
    INFO("sql={}", sql3);
    status = amdb::run(sql3);
    AMDB_ASSERT_EQ(amdb::Status::C_OK, status);
  }

  std::string sql4 = "SELECT id, age FROM db_test.db_table";
  status = amdb::run(sql4);
  AMDB_ASSERT_EQ(amdb::Status::C_OK, status);

  return 0;
}