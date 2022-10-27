#include "sql/chunk/row.h"

#include <gtest/gtest.h>

#include "common/assert.h"

namespace amdb {
namespace chunk {
class RowTest : public testing::Test {
 protected:
  void SetUp() override {
    MemTracker tracker;
    arena_ = new Arena(&tracker);
  }

 protected:
  Arena* arena_;
};

TEST_F(RowTest, SetGet) {
  uint32_t row_id = 0;
  uint32_t col_id_1 = 0;
  uint32_t col_id_2 = 1;

  ColumnDescriptor col_desc1(expr::ExprValueType::UInt8, col_id_1);
  ColumnDescriptor col_desc2(expr::ExprValueType::Int64, col_id_2);

  RowDescriptor row_desc(row_id);
  row_desc.AddColumnDesc(&col_desc1);
  row_desc.AddColumnDesc(&col_desc2);
  row_desc.InitAllColDesc();

  Row row(arena_, &row_desc);

  const expr::ExprValue& v11 = expr::ExprValue::NewUInt8(99);
  Status status = row.SetColValue(row_id, col_id_1, v11);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  const expr::ExprValue& v12 = row.GetColValue(row_id, col_id_1);
  AMDB_ASSERT_EQ(99, v12.u.uint8_value);

  const expr::ExprValue& v21 = expr::ExprValue::NewInt64(-99);
  status = row.SetColValue(row_id, col_id_2, v21);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  const expr::ExprValue& v22 = row.GetColValue(row_id, col_id_2);
  AMDB_ASSERT_EQ(-99, v22.Int64Value());
}
}  // namespace chunk
}  // namespace amdb