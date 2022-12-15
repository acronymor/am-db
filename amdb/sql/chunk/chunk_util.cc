#include "sql/chunk/chunk_util.h"

namespace amdb {
namespace chunk {
chunk::RowDescriptor* ToRowDesc(Arena* arena, schema::TableInfo* table_info) {
  chunk::RowDescriptor* row_desc = arena->CreateObject<chunk::RowDescriptor>(0);
  for (int i = 0; i < table_info->column_list.size(); i++) {
    schema::ColumnInfo& col_info = table_info->column_list.at(i);
    chunk::ColumnDescriptor* col_desc =
        arena->CreateObject<chunk::ColumnDescriptor>(expr::ToExprType(col_info.type), col_info.id);
    row_desc->AddColumnDesc(col_desc);
  }
  row_desc->InitAllColDesc();
  return row_desc;
}
}  // namespace chunk
}  // namespace amdb
