#include "sql/chunk/column.h"

#include <gtest/gtest.h>

#include "common/assert.h"

namespace amdb {
namespace chunk {
class ColumnTest : public testing::Test {
 protected:
  void SetUp() override {
    MemTracker tracker;
    arena_ = new Arena(&tracker);
  }

 protected:
  Arena* arena_;
};

TEST_F(ColumnTest, SetGet) {
  ColumnDescriptor desc(expr::ExprValueType::UInt8, 0);
  int32_t capacity = 5;
  Column column(arena_, &desc, capacity);

  for (uint32_t i = 0; i < capacity; i++) {
    column.SetUInt8(i, 100 - i);
  }

  for (uint32_t i = 0; i < capacity; i++) {
    AMDB_ASSERT_EQ(100 - i, column.GetUInt8(i));
  }
}

}  // namespace chunk
}  // namespace amdb