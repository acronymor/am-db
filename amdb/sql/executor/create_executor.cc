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
  schema::DatabaseInfo* database_info = create_database_plan_->GetTable()->GetDB();

  storage::Metadata meta;
  Status status = meta.DumpDatabaseMeta(database_info->id, database_info);
  RETURN_ERR_NOT_OK(status);

  // save relation of both db_name and db_id, <db_name, db_id>
  status = meta.DumpDatabaseIdByName(database_info->name, &database_info->id);
  RETURN_ERR_NOT_OK(status);
  return Status::C_OK;
}

Status CreateTableExecutor::DoWork() {
  storage::Metadata meta;

  schema::TableInfo* table_info = create_table_plan_->GetTable()->GetTable();
  Status status = meta.LoadDatabaseIdByName(table_info->db_name, &table_info->db_id);
  RETURN_ERR_NOT_OK(status);

  auto add_table = [&table_info](schema::DatabaseInfo* db_info) -> Status {
    db_info->table_ids.insert(table_info->id);
    return Status::C_OK;
  };
  status = meta.ModifyDatabaseMeta(table_info->db_id, add_table);
  RETURN_ERR_NOT_OK(status);

  status = meta.DumpTableMeta(table_info->db_id, table_info->id, table_info);
  RETURN_ERR_NOT_OK(status);

  // save relation of table_name and table_id, <table_name, table_id>
  status = meta.DumpTableIdByName(table_info->db_id, table_info->name, &table_info->id);
  RETURN_ERR_NOT_OK(status);

  return Status::C_OK;
}
}  // namespace executor
}  // namespace amdb
