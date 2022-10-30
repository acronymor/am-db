#include "sql/testsuite/schema_gen_testutil.h"

#include "absl/strings/str_format.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "sql/testsuite/data_gen_testutil.h"

namespace {
std::string toName(uint64_t id, std::string name) {
  return absl::StrFormat("%s-%d", name, id);
}
}  // namespace

namespace amdb {
namespace testsuite {
void SchemaGen::SetUp() { KvDataTest::SetUp(); }

/*
 * db_name = test-db-1
 * table_name = test-table-1
 * id(long) | name(string) | age(short)
 */
schema::TableInfo* SchemaGen::GenTableInfo(uint64_t db_id, uint64_t table_id) {
  table_info_ = arena_->CreateObject<schema::TableInfo>();
  std::string db_name = toName(db_id, "test-db");
  std::string table_name = toName(table_id, "test-table");
  table_info_->column_list.reserve(3);
  table_info_->index_list.reserve(3);

  // ---------------- column list --------------------
  table_info_->column_list.emplace_back();
  schema::ColumnInfo& col_info_01 = table_info_->column_list.at(0);
  col_info_01.id = 0;
  col_info_01.name = "id";
  col_info_01.table_id = table_id;
  col_info_01.table_name = table_name;
  col_info_01.type = {.type = schema::MYSQL_TYPE_LONGLONG,
                      .charset = schema::CharacterSet::CHARSET_UTF8};

  table_info_->column_list.emplace_back();
  schema::ColumnInfo& col_info_02 = table_info_->column_list.at(1);
  col_info_02.id = 1;
  col_info_02.name = "name";
  col_info_02.table_id = table_id;
  col_info_02.table_name = table_name;
  col_info_02.type = {.type = schema::MYSQL_TYPE_STRING,
                      .charset = schema::CharacterSet::CHARSET_UTF8};

  table_info_->column_list.emplace_back();
  schema::ColumnInfo& col_info_03 = table_info_->column_list.at(2);
  col_info_03.id = 2;
  col_info_03.name = "age";
  col_info_03.table_id = table_id;
  col_info_03.table_name = table_name;
  col_info_03.type = {.type = schema::MYSQL_TYPE_SHORT,
                      .charset = schema::CharacterSet::CHARSET_UTF8};

  // ---------------- index list --------------------
  table_info_->index_list.emplace_back();
  schema::IndexInfo& index_info_01 = table_info_->index_list.at(0);
  index_info_01.id = 0;
  index_info_01.table_id = table_id;
  index_info_01.name = "index_id";
  index_info_01.columns.emplace_back(col_info_01);
  index_info_01.type = schema::ConstraintType::CONSTRAINT_PRIMARY;

  table_info_->index_list.emplace_back();
  schema::IndexInfo& index_info_02 = table_info_->index_list.at(1);
  index_info_02.id = 1;
  index_info_02.table_id = table_id;
  index_info_02.name = "index_name";
  index_info_02.columns.emplace_back(col_info_02);
  index_info_02.type = schema::ConstraintType::CONSTRAINT_INDEX;

  table_info_->index_list.emplace_back();
  schema::IndexInfo& index_info_03 = table_info_->index_list.at(2);
  index_info_03.id = 2;
  index_info_03.table_id = table_id;
  index_info_03.name = "index_age";
  index_info_03.columns.emplace_back(col_info_03);
  index_info_03.type = schema::ConstraintType::CONSTRAINT_INDEX;

  // ---------------- table_info --------------------
  table_info_->id = table_id;
  table_info_->name = table_name;
  table_info_->db_id = db_id;
  table_info_->db_name = db_name;
  table_info_->create_ts = absl::ToUnixMicros(absl::Now());
  table_info_->update_ts = absl::ToUnixMicros(absl::Now());

  table_info_->id_to_column.insert({col_info_01.id, &col_info_01});
  table_info_->name_to_column.insert({col_info_01.name, &col_info_01});

  table_info_->primary_index = &index_info_01;
  table_info_->id_to_index.insert({index_info_01.id, &index_info_01});
  table_info_->id_to_index.insert({index_info_02.id, &index_info_02});
  table_info_->id_to_index.insert({index_info_03.id, &index_info_03});
  table_info_->name_to_index.insert({index_info_01.name, &index_info_01});
  table_info_->name_to_index.insert({index_info_02.name, &index_info_02});
  table_info_->name_to_index.insert({index_info_03.name, &index_info_03});
  table_info_->column_id_to_index[col_info_01.id].push_back(&index_info_01);
  table_info_->column_id_to_index[col_info_02.id].push_back(&index_info_02);
  table_info_->column_id_to_index[col_info_03.id].push_back(&index_info_03);

  return table_info_;
}

chunk::RowDescriptor* SchemaGen::GenRowDesc() {
  chunk::ColumnDescriptor* col_desc_1 = arena_->CreateObject<chunk::ColumnDescriptor>(expr::ExprValueType::UInt64, 0);
  chunk::ColumnDescriptor* col_desc_2 = arena_->CreateObject<chunk::ColumnDescriptor>(expr::ExprValueType::String, 1);
  chunk::ColumnDescriptor* col_desc_3 = arena_->CreateObject<chunk::ColumnDescriptor>(expr::ExprValueType::UInt8, 2);

  chunk::RowDescriptor* row_desc = arena_->CreateObject<chunk::RowDescriptor>(0);
  row_desc->AddColumnDesc(col_desc_1);
  row_desc->AddColumnDesc(col_desc_2);
  row_desc->AddColumnDesc(col_desc_3);
  row_desc->InitAllColDesc();
  return row_desc;
}

chunk::Chunk* SchemaGen::GenRows(chunk::RowDescriptor* row_desc, size_t size) {
  chunk::Chunk* rows = arena_->CreateObject<chunk::Chunk>(arena_, row_desc);

  for (size_t i = 0; i < size; i++) {
    expr::ExprValue value1 = expr::ExprValue::NewUInt64(i);
    expr::ExprValue value2 = expr::ExprValue::NewString(toName(i, "col"));
    expr::ExprValue value3 = expr::ExprValue::NewUInt8(i % 99);

    chunk::Row* row = arena_->CreateObject<chunk::Row>(arena_, row_desc);
    row->SetColValue(row_desc->ID(), row_desc->GetColumnDesc(0)->ID(), value1);
    row->SetColValue(row_desc->ID(), row_desc->GetColumnDesc(1)->ID(), value2);
    row->SetColValue(row_desc->ID(), row_desc->GetColumnDesc(2)->ID(), value3);

    rows->AddRow(row);
  }

  return rows;
}
}  // namespace testsuite
}  // namespace amdb