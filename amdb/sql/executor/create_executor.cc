#include "sql/executor/create_executor.h"

#include "sql/storage/id_allocator.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace executor {
Status CreateExecutor::Open() { return Status::C_OK; }

Status CreateExecutor::Close() { return Status::C_OK; }

scheduler::IExecutor::State CreateExecutor::Prepare() { return kNeedData; }

Status CreateExecutor::Work() { return DoWork(); }

Status CreateDatabaseExecutor::DoWork() {
  storage::IdAllocator* id_allocator = storage::IdAllocatorSingleton::GetInstance();

  schema::DatabaseInfo* database_info = create_database_plan_->database_info;

  storage::Metadata meta;
  Status status = meta.DumpDatabaseMeta(database_info->id, database_info);
  RETURN_ERR_NOT_OK(status);

  // save relation of both db_name and db_id, <db_name, db_id>
  status = meta.DumpDatabaseIdByName(database_info->name, &database_info->id);
  RETURN_ERR_NOT_OK(status);
  return Status::C_OK;
}

Status CreateTableExecutor::DoWork() {
  int64_t now = absl::ToUnixMicros(absl::Now());

  schema::TableInfo* table_info = create_table_plan_->table_info;
  table_info->db_id = 0;
  table_info->id = 0;
  table_info->create_ts = now;
  table_info->update_ts = now;

  storage::Metadata meta;
  Status status = meta.DumpTableMeta(table_info->db_id, table_info->id, table_info);
  return status;
}
}  // namespace executor
}  // namespace amdb
