#include "sql/analyzer/insert_analyzer.h"

#include "sql/analyzer/expr_analyzer.h"
#include "sql/planner/logical_plan_node.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace analyzer {
/**
 * Insert database/table
 */
Status InsertAnalyzer::Analyze() {
  planner::LogicalInsert* insert = stmt_ctx_->arena->CreateObject<planner::LogicalInsert>();
  schema::TableInfo* table_info = stmt_ctx_->arena->CreateObject<schema::TableInfo>();
  insert->table_info = table_info;

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

  // table info
  status = meta.LoadTableMeta(db_id, table_id, table_info);
  RETURN_ERR_NOT_OK(status);

  ExprAnalyzer expr_analyzer(stmt_ctx_);

  // value list
  insert->expr_nodes.reserve(stmt_->lists.size());
  for (int i = 0; i < stmt_->lists.size(); i++) {
    auto& children = stmt_->lists[i]->children;

    std::vector<expr::ExprNode*> expr_nodes;
    expr_nodes.reserve(children.size());
    for (int j = 0; j < children.size(); j++) {
      parser::ExprNode* parser_node = dynamic_cast<parser::ExprNode*>(children[j]);
      expr::ExprNode* expr_node = expr_analyzer.AnalyzeNode(parser_node);

      expr_nodes.push_back(expr_node);
    }
    insert->expr_nodes.push_back(expr_nodes);
  }

  stmt_ctx_->logical_plan = insert;
  return Status::C_OK;
}
}  // namespace analyzer
}  // namespace amdb
