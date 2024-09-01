
#include "sql/statistics/statistics.h"

#include <common/assert.h>

#include "gtest/gtest.h"

namespace amdb {
namespace stat {
class StatisticsTest : public testing::Test {
 protected:
  void SetUp() override {
    storage::StorageAPIOptions options;
    Status status = storage::KvStorageAPISingleton::Init(options);
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }
};

TEST_F(StatisticsTest, MetaStatisticLoadAndDump) {
  Statistics statistics;

  MetaStatistic statistic1;
  statistic1.db_id = 1;
  statistic1.table_id = 1;
  statistic1.is_analyzing = false;
  statistic1.create_ts = 1725209777000;
  statistic1.update_ts = 1725209777000;

  Status status1 = statistics.DumpMetaStatistic(1, 1, &statistic1);
  AMDB_ASSERT_EQ(Status::C_OK, status1);

  MetaStatistic statistic2;
  Status status2 = statistics.LoadMetaStatistic(1, 1, &statistic2);
  AMDB_ASSERT_EQ(Status::C_OK, status2);

  AMDB_ASSERT_EQ(statistic1.db_id, statistic2.db_id);
  AMDB_ASSERT_EQ(statistic1.table_id, statistic2.table_id);
  AMDB_ASSERT_EQ(statistic1.is_analyzing, statistic2.is_analyzing);
  AMDB_ASSERT_EQ(statistic1.create_ts, statistic2.create_ts);
  AMDB_ASSERT_EQ(statistic1.update_ts, statistic2.update_ts);
}

TEST_F(StatisticsTest, ColumnStatisticLoadAndDump) {
  Statistics statistics;

  const Histogram::Bucket bucket1 = {
      .start = expr::ExprValue::NewString("a"), .end = expr::ExprValue::NewString("c"), .count = 1, .ndv = 1, .cf = 1};
  const Histogram::Bucket bucket2 = {
      .start = expr::ExprValue::NewString("d"), .end = expr::ExprValue::NewString("f"), .count = 1, .ndv = 1, .cf = 1};
  const Histogram::Bucket bucket3 = {
      .start = expr::ExprValue::NewString("g"), .end = expr::ExprValue::NewString("z"), .count = 1, .ndv = 1, .cf = 1};
  Histogram histogram;
  histogram.AddBucket(bucket1);
  histogram.AddBucket(bucket2);
  histogram.AddBucket(bucket3);

  ColumnStatistic statistic1;
  statistic1.db_id = 1;
  statistic1.table_id = 1;
  statistic1.column_id = 1;
  statistic1.min = "a";
  statistic1.max = "z";
  statistic1.count = 3;
  statistic1.size = 3;
  statistic1.ndv = 3;
  statistic1.histogram = histogram;
  statistic1.create_ts = 1725209777000;
  statistic1.update_ts = 1725209777000;

  Status status1 = statistics.DumpColumnStatistic(1, 1, 1, &statistic1);
  AMDB_ASSERT_EQ(Status::C_OK, status1);

  ColumnStatistic statistic2;
  Status status2 = statistics.LoadColumnStatistic(1, 1, 1, &statistic2);
  AMDB_ASSERT_EQ(Status::C_OK, status2);

  AMDB_ASSERT_EQ(statistic1.db_id, statistic2.db_id);
  AMDB_ASSERT_EQ(statistic1.table_id, statistic2.table_id);
  AMDB_ASSERT_EQ(statistic1.column_id, statistic2.column_id);
  AMDB_ASSERT_EQ(statistic1.create_ts, statistic2.create_ts);
  AMDB_ASSERT_EQ(statistic1.update_ts, statistic2.update_ts);
  AMDB_ASSERT_EQ(statistic1.histogram.GetBuckets()[0].start.StringValue(), statistic2.histogram.GetBuckets()[0].start.StringValue());
  AMDB_ASSERT_EQ(statistic1.histogram.GetBuckets()[0].end.StringValue(), statistic2.histogram.GetBuckets()[0].end.StringValue());
}
}  // namespace stat
}  // namespace amdb