#include "sql/analyzer/create_analyzer.h"

#include "common/const.h"
#include "sql/planner/logical_plan_node.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace analyzer {
Status CreateDatabaseAnalyzer::Analyze() {
  parser::CreateDatabaseStmt* stmt = dynamic_cast<parser::CreateDatabaseStmt*>(stmt_ctx_->stmt_ast);

  schema::DatabaseInfo* database_info = stmt_ctx_->arena->CreateObject<schema::DatabaseInfo>();
  database_info->name = stmt->db_name.to_string();
  database_info->id = amdb::kInvalidIDatabaseID;

  planner::LogicalCreateDatabase* database = stmt_ctx_->arena->CreateObject<planner::LogicalCreateDatabase>();
  database->database_info = database_info;

  stmt_ctx_->logical_plan = database;
  return Status::C_OK;
}

Status CreateTableAnalyzer::Analyze() {
  parser::CreateTableStmt* stmt = dynamic_cast<parser::CreateTableStmt*>(stmt_ctx_->stmt_ast);

  schema::TableInfo* table_info = stmt_ctx_->arena->CreateObject<schema::TableInfo>();
  // table_info->name = stmt->table_name.to_string();
  table_info->name = "Helllo word";
  table_info->id = amdb::kInvalidIDatabaseID;

  planner::LogicalCreateTable* table = stmt_ctx_->arena->CreateObject<planner::LogicalCreateTable>();
  table->table_info = table_info;

  stmt_ctx_->logical_plan = table;
  return Status::C_OK;
}
}  // namespace analyzer
}  // namespace amdb
