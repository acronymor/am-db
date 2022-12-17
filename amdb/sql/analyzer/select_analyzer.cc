#include "sql/analyzer/select_analyzer.h"

#include "sql/planner/logical_plan_node.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace analyzer {
Status SelectAnalyzer::Analyze() {
  planner::LogicalResultSet* result_set = stmt_ctx_->arena->CreateObject<planner::LogicalResultSet>();
  planner::LogicalFilter* filter = stmt_ctx_->arena->CreateObject<planner::LogicalFilter>();
  planner::LogicalTableScan* table_scan = stmt_ctx_->arena->CreateObject<planner::LogicalTableScan>();

  parser::TableSource* table_sources = dynamic_cast<parser::TableSource*>(stmt_->table_refs);
  storage::Metadata meta;

  std::string db_name = table_sources->table_name->db.to_string();
  uint64_t db_id;
  Status status = meta.LoadDatabaseIdByName(db_name, &db_id);
  RETURN_ERR_NOT_OK(status);

  std::string table_name = table_sources->table_name->table.to_string();
  uint64_t table_id;
  status = meta.LoadTableIdByName(db_id, table_name, &table_id);
  RETURN_ERR_NOT_OK(status);

  table_scan->table_info = stmt_ctx_->arena->CreateObject<schema::TableInfo>();
  status = storage::Metadata().LoadTableMeta(db_id, table_id, table_scan->table_info);
  RETURN_ERR_NOT_OK(status);

  for (const schema::ColumnInfo& col_info : table_scan->table_info->primary_index->columns) {
    planner::IndexRange* ir = stmt_ctx_->arena->CreateObject<planner::IndexRange>();
    expr::ExprValue min = expr::ExprValue::NewMin(expr::ToExprType(col_info.type));
    ir->lower.value.push_back(min);
    ir->lower.type = planner::IndexRange::RangePointType::LEFT_CLOSE;

    expr::ExprValue max = expr::ExprValue::NewMax(expr::ToExprType(col_info.type));
    ir->upper.value.push_back(max);
    ir->upper.type = planner::IndexRange::RangePointType::RIGHT_OPEN;

    table_scan->primary_ranges.push_back(ir);
  }

  result_set->AddChild(filter);
  filter->AddChild(table_scan);

  stmt_ctx_->logical_plan = result_set;

  return Status::C_OK;
}
}  // namespace analyzer
}  // namespace amdb
