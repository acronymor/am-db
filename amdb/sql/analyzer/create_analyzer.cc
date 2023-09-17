#include "sql/analyzer/create_analyzer.h"

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "common/assert.h"
#include "common/const.h"
#include "common/log.h"
#include "sql/plan/create_database.h"
#include "sql/plan/create_table.h"
#include "sql/schema/schema.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace analyzer {
Status CreateDatabaseAnalyzer::Analyze() {
  plan::LogicalCreateDatabase* database = stmt_ctx_->arena->CreateObject<plan::LogicalCreateDatabase>();
  schema::DatabaseInfo* database_info = stmt_ctx_->arena->CreateObject<schema::DatabaseInfo>();
  // database->SetDatabaseInfo(database_info);

  std::string db_name = std::string(stmt_->db_name.to_string());
  uint64_t db_id = amdb::kInvalidIDatabaseID;

  storage::Metadata meta;
  Status status = meta.LoadDatabaseIdByName(db_name, &db_id);
  if (Status::C_OK == status) {
    if (!stmt_->if_not_exist) {
      ERROR("Database {} have existed", db_name);
      return Status::C_DATABASE_EXISTED;
    } else {
      DEBUG("Database {} have existed", db_name);
      status = meta.LoadDatabaseMeta(db_id, database_info);
      RETURN_ERR_NOT_OK(status);
      stmt_ctx_->logical_plan = database;
      return Status::C_OK;
    }
  }
  id_allocator->AllocateID(storage::IDType::Database, db_name, &db_id);

  database_info->name = db_name;
  database_info->id = db_id;

  int64_t now = absl::ToUnixMicros(absl::Now());
  database_info->create_ts = now;
  database_info->update_ts = now;

  stmt_ctx_->logical_plan = database;
  return Status::C_OK;
}

Status CreateTableAnalyzer::Analyze() {
  plan::LogicalCreateTable* create_table = stmt_ctx_->arena->CreateObject<plan::LogicalCreateTable>();

  uint64_t db_id = amdb::kInvalidIDatabaseID;
  std::string db_name = std::string(stmt_->table_name->db.to_string());

  storage::Metadata meta;
  Status status = meta.LoadDatabaseIdByName(db_name, &db_id);
  if (status != Status::C_OK) {
    ERROR("Database {} not found", db_name);
    return Status::C_DATABASE_NOT_FOUND;
  }

  schema::DatabaseInfo* database_info = stmt_ctx_->arena->CreateObject<schema::DatabaseInfo>();
  status = meta.LoadDatabaseMeta(db_id, database_info);
  RETURN_ERR_NOT_OK(status);

  std::string table_name = std::string(stmt_->table_name->table.to_string());
  uint64_t table_id = amdb::kInvalidTableID;

  status = meta.LoadTableIdByName(db_id, table_name, &table_id);
  schema::TableInfo* table_info = stmt_ctx_->arena->CreateObject<schema::TableInfo>();
  if (Status::C_OK == status) {
    if (!stmt_->if_not_exist) {
      ERROR("Table {} have existed", table_name);
      return Status::C_TABLE_EXISTE;
    } else {
      DEBUG("Table {} have existed", table_name);
      status = meta.LoadTableMeta(db_id, table_id, table_info);
      RETURN_ERR_NOT_OK(status);
      stmt_ctx_->logical_plan = create_table;
      return Status::C_OK;
    }
  }
  plan::RelOptTable* table = stmt_ctx_->arena->CreateObject<plan::RelOptTable>();
  table->Init(database_info, table_info);
  create_table->SetTable(table);

  id_allocator->AllocateID(storage::IDType::Table, table_name, &table_id);
  int64_t now = absl::ToUnixMicros(absl::Now());

  table_info->db_id = db_id;
  table_info->db_name = db_name;
  table_info->id = table_id;
  table_info->name = table_name;
  table_info->create_ts = now;
  table_info->update_ts = now;

  // init table_info->column_list
  for (size_t i = 0; i < stmt_->columns.size(); i++) {
    parser::ColumnDef* col_def = stmt_->columns[i];

    schema::ColumnInfo* col_info = stmt_ctx_->arena->CreateObject<schema::ColumnInfo>();
    col_info->id = i;
    col_info->name = col_def->name->name.to_string();
    col_info->table_id = table_id;
    col_info->table_name = table_name;
    col_info->type.type = col_def->type->type;
    col_info->type.charset = parser::CHARSET_UTF8;
    col_info->comment = "hello world";

    table_info->column_list.push_back(*col_info);
    table_info->id_to_column.emplace(col_info->id, col_info);
    table_info->name_to_column.emplace(col_info->name, col_info);
  }

  for (size_t i = 0; i < stmt_->constraints.size(); i++) {
    parser::Constraint* cons_def = stmt_->constraints[i];

    schema::IndexInfo* index_info = stmt_ctx_->arena->CreateObject<schema::IndexInfo>();
    index_info->id = i;
    index_info->name = cons_def->name.to_string();
    index_info->table_id = table_id;
    index_info->table_name = table_name;
    index_info->root_node_id = 0;
    index_info->max_tree_node_id = 0;
    index_info->comment = "hello world";
    index_info->type = cons_def->type;

    index_info->columns.reserve(cons_def->columns.size());
    for (size_t j = 0; j < cons_def->columns.size(); j++) {
      parser::ColumnName* row_col_name = cons_def->columns[j];
      std::string col_name(row_col_name->name.to_string());
      auto itr = table_info->name_to_column.find(col_name);
      index_info->columns.push_back(*itr->second);
    }

    table_info->index_list.push_back(*index_info);
    table_info->id_to_index.emplace(index_info->id, index_info);
    table_info->name_to_index.emplace(index_info->name, index_info);
  }

  stmt_ctx_->logical_plan = create_table;

  return Status::C_OK;
}
}  // namespace analyzer
}  // namespace amdb
