#include "sql/testsuite/table_gen_testutil.h"

#include "sql/chunk/chunk_util.h"
#include "sql/expression/expr_value_type.h"

namespace amdb {
namespace testsuite {
storage::Table* TableGen::GetTable(uint64_t db_id, uint64_t table_id) {
  std::string key = toTableKey(db_id, table_id);
  if (auto itr = tables_.find(key); itr != tables_.end()) {
    return itr->second;
  }

  schema::TableInfo* table_info = GenTableInfo(db_id, table_id);
  table_ = GetArena()->CreateObject<storage::Table>(GetArena(), table_info);
  tables_.emplace(key, table_);
  return table_;
}

chunk::Chunk* TableGen::GenRows(schema::TableInfo* table_info, size_t size) {
  chunk::RowDescriptor* row_desc = chunk::ToRowDesc(arena_, table_info);

  chunk::Chunk* rows = arena_->CreateObject<chunk::Chunk>(arena_);
  rows->SetRowDesc(row_desc);

  for (size_t i = 0; i < size; i++) {
    data_gen_->NextSeq();

    chunk::Row* row = arena_->CreateObject<chunk::Row>(arena_, row_desc);
    rows->AddRow(row);

    for (size_t j = 0; j < table_info->column_list.size(); j++) {
      schema::ColumnInfo& col_info = table_info->column_list.at(j);
      row->SetColValue(row_desc->ID(), row_desc->GetColumnDesc(j)->ID(), toExprValue(col_info.type));
    }
  }

  return rows;
}

expr::ExprValue TableGen::toExprValue(schema::ColumnType type) {
  expr::ExprValue value = expr::ExprValue::NewNull();

  expr::ExprValueType expr_type = expr::ToExprType(type);
  switch (expr_type) {
    case expr::ExprValueType::Int32:
      value = data_gen_->GenInt32Value();
      break;
    case expr::ExprValueType::UInt64:
      value = data_gen_->GenUint64Value();
      break;
    case expr::ExprValueType::Int64:
      value = data_gen_->GenInt64Value();
      break;
    case expr::ExprValueType::String:
      value = data_gen_->GenStringValue();
      break;
    default:
      ERROR("Not Support type, {}", expr_type);
      break;
  }

  return value;
}

}  // namespace testsuite
}  // namespace amdb
