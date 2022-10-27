#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/testsuite/data_gen_testutil.h"

namespace amdb {
namespace testutil {
class DataGenTest : public ::testing::Test {
 protected:
  void SetUp() override {
    random_ = new RandomDataGen();
    seq_ = new SeqDataGen();
  }

  void TearDown() override { delete random_; }

 protected:
  RandomDataGen* random_;
  SeqDataGen* seq_;
};

TEST_F(DataGenTest, Random) {
  expr::ExprValue uint64_value = random_->GenUint64Value();
  AMDB_ASSERT_EQ(expr::ExprValueType::UInt64, uint64_value.Type());

  expr::ExprValue int64_value = random_->GenInt64Value();
  AMDB_ASSERT_EQ(expr::ExprValueType::Int64, int64_value.Type());

  expr::ExprValue string_value = random_->GenStringValue();
  AMDB_ASSERT_EQ(expr::ExprValueType::String, string_value.Type());
};

TEST_F(DataGenTest, Seq) {
  expr::ExprValue uint64_value_1 = seq_->GenUint64Value();
  AMDB_ASSERT_EQ(0, uint64_value_1.UInt64Value());

  seq_->NextSeq(1);
  expr::ExprValue uint64_value_2 = seq_->GenUint64Value();
  AMDB_ASSERT_EQ(1, uint64_value_2.UInt64Value());

  seq_->NextSeq(10);
  expr::ExprValue uint64_value_3 = seq_->GenUint64Value();
  AMDB_ASSERT_EQ(11, uint64_value_3.UInt64Value());
};
}  // namespace testutil
}  // namespace amdb