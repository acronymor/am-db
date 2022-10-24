#include "sql/storage/table.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/storage/kv_data_testutil.h"

namespace amdb {
namespace storage {
class TableTest : public KvDataTest {
 protected:
  void SetUp() override {
    KvDataTest::SetUp();

    TreeCtx::Schema schema1 = {.db_id = 0, .table_id = 0, .index_id = 0};
    tree_ctx_1 = new TreeCtx(instance_, arena_, schema1);
    index_root_1.set_id(tree_ctx_1->AllocateNodeID());

    col_info_1.id = 0;
    col_info_1.table_id = schema1.table_id;
    col_info_1.name = "id";

    index_info_1.id = schema1.index_id;
    index_info_1.table_id = schema1.table_id;
    index_info_1.name = "index_id";
    index_info_1.columns.push_back(col_info_1);
    index_info_1.type = schema::ConstraintType::CONSTRAINT_PRIMARY;

    //---------------------------

    TreeCtx::Schema schema2 = {.db_id = 0, .table_id = 0, .index_id = 1};
    tree_ctx_2 = new TreeCtx(instance_, arena_, schema2);
    index_root_2.set_id(tree_ctx_2->AllocateNodeID());

    col_info_2.id = 1;
    col_info_2.table_id = 0;
    col_info_2.name = "age";

    index_info_2.id = schema2.index_id;
    index_info_2.table_id = schema2.table_id;
    index_info_2.name = "index_age";
    index_info_2.columns.push_back(col_info_1);
    index_info_2.type = schema::ConstraintType::CONSTRAINT_INDEX;

    //---------------------------

    table_info.id = 0;
    table_info.db_id = 0;
    table_info.name = "test";
    table_info.db_name = "db";
    table_info.column_list.push_back(col_info_1);
    table_info.column_list.push_back(col_info_2);
    table_info.id_to_column.insert({col_info_1.id, &col_info_1});
    table_info.name_to_column.insert({col_info_1.name, &col_info_1});
    table_info.index_list.push_back(index_info_1);
    table_info.index_list.push_back(index_info_2);
    table_info.id_to_index.insert({index_info_1.id, &index_info_1});
    table_info.id_to_index.insert({index_info_2.id, &index_info_2});
    table_info.name_to_index.insert({index_info_1.name, &index_info_1});
    table_info.name_to_index.insert({index_info_2.name, &index_info_2});
    table_info.column_id_to_index[col_info_1.id].push_back(&index_info_1);
    table_info.column_id_to_index[col_info_2.id].push_back(&index_info_2);
    table_info.primary_index = &index_info_1;

    table_ = new Table(arena_, &table_info, instance_);
  }

 protected:
  Table* table_;

 private:
  schema::ColumnInfo col_info_1;
  schema::ColumnInfo col_info_2;
  schema::IndexInfo index_info_1;
  schema::IndexInfo index_info_2;
  schema::TableInfo table_info;
  BptNonLeafNodeProto index_root_1;
  BptNonLeafNodeProto index_root_2;

  TreeCtx* tree_ctx_1;
  TreeCtx* tree_ctx_2;
};

TEST_F(TableTest, CURD) {
  chunk::ColumnDescriptor col_desc_1(expr::ExprValueType::UInt32, 0);
  chunk::ColumnDescriptor col_desc_2(expr::ExprValueType::UInt8, 1);

  chunk::RowDescriptor row_desc(0);
  row_desc.AddColumnDesc(&col_desc_1);
  row_desc.AddColumnDesc(&col_desc_2);

  chunk::Row row(arena_, &row_desc);
  expr::ExprValue value1 = expr::ExprValue::NewUInt32(1);
  expr::ExprValue value2 = expr::ExprValue::NewUInt8(18);
  row.SetColValue(row_desc.ID(), col_desc_1.ID(), value1);
  row.SetColValue(row_desc.ID(), col_desc_2.ID(), value2);

  chunk::Chunk rows(arena_, &row_desc);
  rows.AddRow(&row);
  Status status = table_->Insert(&rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);
};
}  // namespace storage
}  // namespace amdb