#include "sql/codec/codec.h"
#include "sql/codec/rc_codec.h"

namespace amdb {
namespace codec {
/**
 * primary key: <id, "">
 * column key: <pk_id + id, "">
 */
size_t EncodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, chunk::Row* row,
                   std::string* key, std::string* value) {
  *value = "";

  auto encode = [&row, &key](const schema::ColumnInfo* col_info) -> void {
    expr::ExprValue col_key = row->GetColValue(0, col_info->id);
    EncodeUInt8(col_key.Type(), key);
    EncodeExprValue(col_key, key);
  };

  if (table_info->primary_index->type != index_info->type) {
    for (const schema::ColumnInfo& col_info : index_info->columns) {
      encode(&col_info);
    }
  }

  for (schema::ColumnInfo& col_info : table_info->primary_index->columns) {
    encode(&col_info);
  }

  return table_info->column_list.size();
}

size_t DecodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, std::string* key,
                   std::string* value, chunk::Row* row) {
  *value = "";

  auto decode = [&row, &key](const schema::ColumnInfo* col_info) -> void {
    uint32_t offset = 0;

    uint8_t type;
    offset += DecodeUInt8(*key, &type);
    expr::ExprValue expr_value =
        expr::ExprValue::NewEmpty((expr::ExprValueType)type);

    std::string tmp;
    tmp.assign(tmp.data() + offset, expr_value.Length());
    DecodeExprValue(tmp, &expr_value);
    row->SetColValue(0, col_info->id, expr_value);
  };

  if (table_info->primary_index->type != index_info->type) {
    for (const schema::ColumnInfo& col_info : index_info->columns) {
      decode(&col_info);
    }
  }

  for (const schema::ColumnInfo& col_info : table_info->primary_index->columns) {
    decode(&col_info);
  }

  return table_info->column_list.size();
}

// <id, [name,age,sex]>
size_t EncodeRow(const schema::TableInfo* table_info, chunk::Row* row,
                 std::string* key, std::string* value) {
  // encode key
  EncodeIndex(table_info, table_info->primary_index, row, key, value);

  // encode value
  for (const schema::ColumnInfo& col_info : table_info->column_list) {
    expr::ExprValue expr_value = row->GetColValue(0, col_info.id);

    codec::EncodeUInt8(expr_value.Type(), value);
    codec::EncodeExprValue(expr_value, value);
  }

  return 0;
}
}  // namespace codec
}  // namespace amdb