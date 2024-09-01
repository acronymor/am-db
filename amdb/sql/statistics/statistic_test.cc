#include "sql/statistics/statistic.h"

#include <common/assert.h>

#include "gtest/gtest.h"

namespace amdb {
namespace stat {
class ColumnStatisticTest : public testing::Test {
 protected:
  void SetUp() override {
    const Histogram::Bucket bucket1 = {.start = expr::ExprValue::NewString("a"),
                                       .end = expr::ExprValue::NewString("c"),
                                       .count = 5,
                                       .ndv = 3,
                                       .cf = 3};
    const Histogram::Bucket bucket2 = {.start = expr::ExprValue::NewString("e"),
                                       .end = expr::ExprValue::NewString("h"),
                                       .count = 3,
                                       .ndv = 1,
                                       .cf = 4};
    const Histogram::Bucket bucket3 = {.start = expr::ExprValue::NewString("i"),
                                       .end = expr::ExprValue::NewString("l"),
                                       .count = 7,
                                       .ndv = 2,
                                       .cf = 6};
    const Histogram::Bucket bucket4 = {.start = expr::ExprValue::NewString("m"),
                                       .end = expr::ExprValue::NewString("o"),
                                       .count = 9,
                                       .ndv = 7,
                                       .cf = 13};
    const Histogram::Bucket bucket5 = {.start = expr::ExprValue::NewString("p"),
                                       .end = expr::ExprValue::NewString("z"),
                                       .count = 2,
                                       .ndv = 1,
                                       .cf = 14};

    histogram_.AddBucket(bucket1);
    histogram_.AddBucket(bucket2);
    histogram_.AddBucket(bucket3);
    histogram_.AddBucket(bucket4);
    histogram_.AddBucket(bucket5);
  }

 protected:
  Histogram histogram_;
};

TEST_F(ColumnStatisticTest, SerializeAndDeserialize) {
  ColumnStatistic cs1 = {.db_id = 1,
                          .table_id = 2,
                          .column_id = 1,
                          .min = "0",
                          .max = "99",
                          .size = 99,
                          .ndv = 40,
                          .create_ts = 1722526404000,
                          .update_ts = 1724081604486,
                          .histogram = histogram_};

  ColumnStatisticsProto proto;
  Status status1 = cs1.Serialize(&proto);
  AMDB_ASSERT_EQ(Status::C_OK, status1);

  ColumnStatistic cs2;
  Status status2 = cs2.Deserialize(proto);
  AMDB_ASSERT_EQ(Status::C_OK, status2);

  AMDB_ASSERT_EQ(cs1.db_id, cs2.db_id);
  AMDB_ASSERT_EQ(cs1.create_ts, cs2.create_ts);
  AMDB_ASSERT_EQ(cs1.histogram.GetBuckets().size(), cs2.histogram.GetBuckets().size());
  AMDB_ASSERT_EQ(cs1.histogram.GetBuckets()[0].start.StringValue(), cs2.histogram.GetBuckets()[0].start.StringValue());
  AMDB_ASSERT_EQ(cs1.histogram.GetBuckets()[0].count, cs2.histogram.GetBuckets()[0].count);
}
}  // namespace stat
}  // namespace amdb
