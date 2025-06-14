#include "sql/analyzer/select_analyzer.h"

#include "common/assert.h"
#include "sql/plan/filter.h"
#include "sql/plan/range.h"
#include "sql/plan/result_set.h"
#include "sql/plan/table_scan.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace analyzer {
Status SelectAnalyzer::Analyze() {
  storage::Metadata meta;
  const parser::TableSource* table_sources = dynamic_cast<parser::TableSource*>(stmt_->table_refs);

  const std::string& db_name = table_sources->table_name->db.to_string();
  uint64_t db_id = kInvalidIDatabaseID;
  Status status = meta.LoadDatabaseIdByName(db_name, &db_id);
  RETURN_ERR_NOT_OK(status);

  schema::DatabaseInfo* database_info = stmt_ctx_->arena->CreateObject<schema::DatabaseInfo>();
  status = meta.LoadDatabaseMeta(db_id, database_info);
  RETURN_ERR_NOT_OK(status);

  const std::string& table_name = table_sources->table_name->table.to_string();
  uint64_t table_id = kInvalidTableID;
  status = meta.LoadTableIdByName(db_id, table_name, &table_id);
  RETURN_ERR_NOT_OK(status);

  schema::TableInfo* table_info = stmt_ctx_->arena->CreateObject<schema::TableInfo>();
  status = storage::Metadata().LoadTableMeta(db_id, table_id, table_info);
  RETURN_ERR_NOT_OK(status);

  std::vector<plan::IndexRange*> index_ranges;
  for (const schema::ColumnInfo& col_info : table_info->primary_index->columns) {
    plan::IndexRange* ir = stmt_ctx_->arena->CreateObject<plan::IndexRange>();
    expr::ExprValue min = expr::ExprValue::NewMin(expr::ToExprType(col_info.type));
    ir->lower.value.push_back(min);
    ir->lower.type = plan::IndexRange::RangePointType::LEFT_CLOSE;

    expr::ExprValue max = expr::ExprValue::NewMax(expr::ToExprType(col_info.type));
    ir->upper.value.push_back(max);
    ir->upper.type = plan::IndexRange::RangePointType::RIGHT_OPEN;
    index_ranges.push_back(ir);
  }

  plan::RelOptTable* table = stmt_ctx_->arena->CreateObject<plan::RelOptTable>();
  table->Init(database_info, table_info);

  plan::LogicalTableScan* table_scan = stmt_ctx_->arena->CreateObject<plan::LogicalTableScan>();
  table_scan->SetTable(table);
  table_scan->PrimaryRanges()->assign(index_ranges.begin(), index_ranges.end());

  plan::LogicalFilter* filter = stmt_ctx_->arena->CreateObject<plan::LogicalFilter>();
  filter->AddInput(table_scan);

  plan::LogicalResultSet* result_set = stmt_ctx_->arena->CreateObject<plan::LogicalResultSet>();
  result_set->AddInput(filter);

  stmt_ctx_->logical_plan = result_set;
  return Status::C_OK;
}
}  // namespace analyzer
}  // namespace amdb
