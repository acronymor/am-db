#include "sql/codec/schema_codec.h"
#include "sql/codec/rc_codec.h"
#include "sql/codec/codec.h"

namespace amdb {
namespace codec {
/**
 * primary key: <id, "">
 * column key: <id + pk_id, "">
 */
size_t EncodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, chunk::Row* row,
                   std::string* key, std::string* value) {
  *value = "";

  auto encode = [&row, &key](const schema::ColumnInfo* col_info) -> void {
    expr::ExprValue col_key = row->GetColValue(row->desc->ID(), col_info->id);
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

  return 0;
}

size_t DecodeIndex(const schema::TableInfo* table_info,
                   const schema::IndexInfo* index_info, std::string* key,
                   std::string* value, chunk::Row* row) {
  *value = "";

  auto decode = [&row, &key](const schema::ColumnInfo* col_info) -> void {
    uint32_t offset = 0;

    std::string tmp;
    uint8_t type;

    tmp.assign(key->data() + offset, sizeof(type));
    offset += DecodeUInt8(tmp, &type);

    expr::ExprValue expr_value =
        expr::ExprValue::NewEmpty((expr::ExprValueType)type);

    tmp.clear();
    tmp.assign(key->data() + offset, expr_value.Length());
    offset += DecodeExprValue(tmp, &expr_value);
    row->SetColValue(row->desc->ID(), col_info->id, expr_value);
  };

  if (table_info->primary_index->type != index_info->type) {
    for (const schema::ColumnInfo& col_info : index_info->columns) {
      decode(&col_info);
    }
  }

  for (const schema::ColumnInfo& col_info :
       table_info->primary_index->columns) {
    decode(&col_info);
  }

  return 0;
}

// <id, [name,age,sex]>
size_t EncodeRow(const schema::TableInfo* table_info, chunk::Row* row,
                 std::string* key, std::string* value) {
  // encode key
  for (schema::ColumnInfo& col_info : table_info->primary_index->columns) {
    std::string col_key =
        row->GetColValue(row->desc->ID(), col_info.id).ToString();
    EncodeDataKey(table_info->db_id, table_info->id, col_key, key);
  }

  // encode value
  for (const schema::ColumnInfo& col_info : table_info->column_list) {
    expr::ExprValue expr_value = row->GetColValue(row->desc->ID(), col_info.id);

    codec::EncodeUInt8(expr_value.Type(), value);
    codec::EncodeExprValue(expr_value, value);
  }

  return 0;
}

size_t DecodeRow(Arena* arena, const schema::TableInfo* table_info,
                 std::string* value, chunk::Row* row) {
  // decode value
  size_t offset = 0;
  for (const schema::ColumnInfo& col_info : table_info->column_list) {
    uint8_t type;

    std::string tmp;
    tmp.assign(value->data() + offset, sizeof(type));
    offset += codec::DecodeUInt8(tmp, &type);

    expr::ExprValue expr_value =
        expr::ExprValue::NewEmpty(expr::ExprValueType(type));

    tmp.clear();
    tmp.assign(value->data() + offset, value->length() - offset);

    offset += codec::DecodeExprValue(tmp, &expr_value);
    row->SetColValue(row->desc->ID(), col_info.id, expr_value);
  }

  return 0;
}
}  // namespace codec
}  // namespace amdb