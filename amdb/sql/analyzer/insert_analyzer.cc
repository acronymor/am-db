#include "sql/analyzer/insert_analyzer.h"

#include "sql/analyzer/expr_analyzer.h"
#include "sql/plan/insert.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace analyzer {
/**
 * Insert database/table
 */
Status InsertAnalyzer::Analyze() {
  // database id
  uint64_t db_id = amdb::kInvalidIDatabaseID;
  std::string db_name = std::string(stmt_->table_name->db.to_string());

  storage::Metadata meta;
  Status status = meta.LoadDatabaseIdByName(db_name, &db_id);
  if (status != Status::C_OK) {
    ERROR("Database {} not found", db_name);
    return Status::C_DATABASE_NOT_FOUND;
  }

  // table id
  std::string table_name = std::string(stmt_->table_name->table.to_string());
  uint64_t table_id = amdb::kInvalidTableID;

  status = meta.LoadTableIdByName(db_id, table_name, &table_id);
  if (status != Status::C_OK) {
    ERROR("Table {} not found", db_name);
    return Status::C_TABLE_NOT_FOUND;
  }

  // database info
  schema::DatabaseInfo* database_info = stmt_ctx_->arena->CreateObject<schema::DatabaseInfo>();
  status = meta.LoadDatabaseMeta(db_id, database_info);
  RETURN_ERR_NOT_OK(status);

  // table info
  schema::TableInfo* table_info = stmt_ctx_->arena->CreateObject<schema::TableInfo>();
  status = meta.LoadTableMeta(db_id, table_id, table_info);
  RETURN_ERR_NOT_OK(status);

  plan::RelOptTable* table = stmt_ctx_->arena->CreateObject<plan::RelOptTable>();
  table->Init(database_info, table_info);

  plan::LogicalInsert* insert = stmt_ctx_->arena->CreateObject<plan::LogicalInsert>();
  insert->SetTable(table);

  ExprAnalyzer expr_analyzer(stmt_ctx_);

  // value list
  insert->ExprNodes().reserve(stmt_->lists.size());
  for (int i = 0; i < stmt_->lists.size(); i++) {
    auto& children = stmt_->lists[i]->children;

    std::vector<expr::ExprNode*> expr_nodes;
    expr_nodes.reserve(children.size());
    for (int j = 0; j < children.size(); j++) {
      parser::ExprNode* parser_node = dynamic_cast<parser::ExprNode*>(children[j]);
      expr::ExprNode* expr_node = expr_analyzer.AnalyzeNode(parser_node);

      expr_nodes.push_back(expr_node);
    }
    insert->ExprNodes().push_back(expr_nodes);
  }

  stmt_ctx_->logical_plan = insert;
  return Status::C_OK;
}
}  // namespace analyzer
}  // namespace amdb
