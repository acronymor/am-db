#include "sql/planner/range_cmp.h"

#include "common/assert.h"
#include "gtest/gtest.h"

namespace amdb {
namespace planner {
struct RangeCmpTest : public testing::Test {
 protected:
  void SetUp() override { arena_ = new Arena(nullptr); }

  void TearDown() override { delete arena_, arena_ = nullptr; }

 protected:
  Arena* arena_;
};

TEST_F(RangeCmpTest, UpTest) {
  IndexRange ir1;
  ir1.lower.value.push_back(expr::ExprValue::NewUInt8(1));
  ir1.lower.value.push_back(expr::ExprValue::NewString("a", arena_));
  ir1.lower.type = IndexRange::RangePointType::LEFT_CLOSE;
  ir1.lower_str = "";
  ir1.upper.value.push_back(expr::ExprValue::NewUInt8(3));
  ir1.upper.value.push_back(expr::ExprValue::NewString("c", arena_));
  ir1.upper.type = IndexRange::RangePointType::RIGHT_OPEN;
  ir1.upper_str = "";

  IndexRange ir2;
  ir2.lower.value.push_back(expr::ExprValue::NewUInt8(1));
  ir2.lower.value.push_back(expr::ExprValue::NewString("b", arena_));
  ir2.lower.type = IndexRange::RangePointType::LEFT_CLOSE;
  ir2.lower_str = "";
  ir2.upper.value.push_back(expr::ExprValue::NewUInt8(3));
  ir2.upper.value.push_back(expr::ExprValue::NewString("c", arena_));
  ir2.upper.type = IndexRange::RangePointType::RIGHT_OPEN;
  ir2.upper_str = "";

  AMDB_ASSERT_TRUE(common::IsCmpLt(IndexRangeCmp(&ir1, &ir2)));
}
}  // namespace planner
}  // namespace amdb