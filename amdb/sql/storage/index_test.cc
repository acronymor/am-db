#include "sql/storage/index.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/testsuite/kv_data_testutil.h"

namespace amdb {
namespace storage {
class IndexTest : public testsuite::KvDataTest {
 protected:
  void SetUp() override {
    KvDataTest::SetUp();

    TreeCtx::Schema schema = {.db_id = 0, .table_id = 0, .index_id = 0};
    tree_ctx_ = new TreeCtx(instance_, arena_, schema);

    BptNodeProto root;
    root.set_id(tree_ctx_->AllocateNodeID());
    bptree_ = new Bptree(tree_ctx_, &root);

    col_info_1.id = 0;
    col_info_1.table_id = 0;
    col_info_1.name = "id";

    col_info_2.id = 1;
    col_info_2.table_id = 0;
    col_info_2.name = "age";

    index_info.id = 0;
    index_info.table_id = 0;
    index_info.name = "id";
    index_info.name = "test";
    index_info.columns.push_back(col_info_1);
    index_info.type = schema::ConstraintType::CONSTRAINT_PRIMARY;

    table_info.id = 0;
    table_info.db_id = 0;
    table_info.name = "test";
    table_info.db_name = "db";
    table_info.column_list.push_back(col_info_1);
    table_info.column_list.push_back(col_info_2);
    table_info.id_to_column.insert({col_info_1.id, &col_info_1});
    table_info.name_to_column.insert({col_info_1.name, &col_info_1});
    table_info.index_list.push_back(index_info);
    table_info.id_to_index.insert({index_info.id, &index_info});
    table_info.name_to_index.insert({index_info.name, &index_info});
    table_info.column_id_to_index[col_info_1.id].push_back(&index_info);
    table_info.primary_index = &index_info;

    root.set_id(tree_ctx_->AllocateNodeID());
    index_ = new Index(instance_, arena_, &table_info, &index_info, &root);
  };

 protected:
  Index* index_;

 private:
  schema::ColumnInfo col_info_1;
  schema::ColumnInfo col_info_2;
  schema::IndexInfo index_info;
  schema::TableInfo table_info;
  BptNodeProto root;
  TreeCtx* tree_ctx_;
  Bptree* bptree_;

 protected:
  void TearDown() override {
    delete tree_ctx_;
    delete bptree_;
    delete index_;

    KvDataTest::TearDown();
  }
};

TEST_F(IndexTest, CURD) {
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

  Status status = index_->Insert(&rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = index_->Save();
  AMDB_ASSERT_EQ(Status::C_OK, status);
};
}  // namespace storage
}  // namespace amdb