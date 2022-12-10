#include "sql/analyzer/create_analyzer.h"

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "common/const.h"
#include "sql/planner/logical_plan_node.h"
#include "sql/schema/schema.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace analyzer {
Status CreateDatabaseAnalyzer::Analyze() {
  parser::CreateDatabaseStmt* stmt = dynamic_cast<parser::CreateDatabaseStmt*>(stmt_ctx_->stmt_ast);

  std::string db_name = std::string(stmt->db_name.to_string());
  uint64_t db_id = amdb::kInvalidIDatabaseID;
  id_allocator->AllocateID(storage::IDType::Database, db_name, &db_id);

  schema::DatabaseInfo* database_info = stmt_ctx_->arena->CreateObject<schema::DatabaseInfo>();
  database_info->name = db_name;
  database_info->id = db_id;

  planner::LogicalCreateDatabase* database = stmt_ctx_->arena->CreateObject<planner::LogicalCreateDatabase>();

  int64_t now = absl::ToUnixMicros(absl::Now());
  database_info->create_ts = now;
  database_info->update_ts = now;
  database->database_info = database_info;

  stmt_ctx_->logical_plan = database;
  return Status::C_OK;
}

Status CreateTableAnalyzer::Analyze() {
  parser::CreateTableStmt* stmt = dynamic_cast<parser::CreateTableStmt*>(stmt_ctx_->stmt_ast);

  uint64_t db_id = amdb::kInvalidIDatabaseID;
  std::string db_name = std::string(stmt->table_name->db.to_string());

  storage::Metadata meta;
  Status status = meta.LoadDatabaseIdByName(db_name, &db_id);
  if (status != Status::C_OK) {
    ERROR("Load database id by name failed");
  }

  std::string table_name = std::string(stmt->table_name->table.to_string());
  uint64_t table_id = amdb::kInvalidTableID;
  id_allocator->AllocateID(storage::IDType::Table, table_name, &table_id);

  int64_t now = absl::ToUnixMicros(absl::Now());

  schema::TableInfo* table_info = stmt_ctx_->arena->CreateObject<schema::TableInfo>();
  table_info->db_id = db_id;
  table_info->db_name = db_name;
  table_info->id = table_id;
  table_info->name = table_name;
  table_info->create_ts = now;
  table_info->update_ts = now;

  // init table_info->column_list
  for (size_t i = 0; i < stmt->columns.size(); i++) {
    parser::ColumnDef* col_def = stmt->columns[i];

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

  for (size_t i = 0; i < stmt->constraints.size(); i++) {
    parser::Constraint* cons_def = stmt->constraints[i];

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

  planner::LogicalCreateTable* table = stmt_ctx_->arena->CreateObject<planner::LogicalCreateTable>();
  table->table_info = table_info;

  stmt_ctx_->logical_plan = table;
  return Status::C_OK;
}
}  // namespace analyzer
}  // namespace amdb
