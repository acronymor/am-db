#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace storage {
class IndexTest : public testsuite::TableDataGen {};

TEST_F(IndexTest, CURD) {
  chunk::ColumnDescriptor col_desc_1(expr::ExprValueType::UInt32, 0);
  chunk::ColumnDescriptor col_desc_2(expr::ExprValueType::UInt8, 1);

  chunk::RowDescriptor row_desc(0);
  row_desc.AddColumnDesc(&col_desc_1);
  row_desc.AddColumnDesc(&col_desc_2);

  chunk::Row row1(table_gen.GetArena(), &row_desc);
  expr::ExprValue value1 = expr::ExprValue::NewUInt32(1);
  expr::ExprValue value2 = expr::ExprValue::NewUInt8(18);
  row1.SetColValue(row_desc.ID(), col_desc_1.ID(), value1);
  row1.SetColValue(row_desc.ID(), col_desc_2.ID(), value2);

  chunk::Chunk rows1(table_gen.GetArena());
  rows1.SetRowDesc(&row_desc);
  rows1.AddRow(&row1);

  Status status = table_->row_index->Insert(&rows1);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = table_->row_index->Save();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  chunk::Row row2(table_gen.GetArena(), &row_desc);
  row2.SetColValue(row_desc.ID(), col_desc_1.ID(), value1);
  row2.SetColValue(row_desc.ID(), col_desc_2.ID(), value2);

  chunk::Chunk rows2(table_gen.GetArena());
  rows2.SetRowDesc(&row_desc);
  rows2.AddRow(&row2);

  status = table_->row_index->Delete(&rows2);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = table_->row_index->Save();
  AMDB_ASSERT_EQ(Status::C_OK, status);
};
}  // namespace storage
}  // namespace amdb