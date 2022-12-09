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
