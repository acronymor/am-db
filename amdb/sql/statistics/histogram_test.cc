#include <common/assert.h>

#include <map>

#include "gtest/gtest.h"
#include "sql/statistics/statistic.h"

namespace amdb {
namespace stat {
class HistogramTest : public testing::Test {};

TEST_F(HistogramTest, Basic) {
  const Histogram::Bucket bucket1 = {
      .start = expr::ExprValue::NewString("a"), .end = expr::ExprValue::NewString("c"), .count = 5, .ndv = 3, .cf = 3};
  const Histogram::Bucket bucket2 = {
      .start = expr::ExprValue::NewString("e"), .end = expr::ExprValue::NewString("h"), .count = 3, .ndv = 1, .cf = 4};
  const Histogram::Bucket bucket3 = {
      .start = expr::ExprValue::NewString("i"), .end = expr::ExprValue::NewString("l"), .count = 7, .ndv = 2, .cf = 6};
  const Histogram::Bucket bucket4 = {
      .start = expr::ExprValue::NewString("m"), .end = expr::ExprValue::NewString("o"), .count = 9, .ndv = 7, .cf = 13};
  const Histogram::Bucket bucket5 = {
      .start = expr::ExprValue::NewString("p"), .end = expr::ExprValue::NewString("z"), .count = 2, .ndv = 1, .cf = 14};

  Histogram histogram;
  histogram.AddBucket(bucket1);
  histogram.AddBucket(bucket2);
  histogram.AddBucket(bucket3);
  histogram.AddBucket(bucket4);
  histogram.AddBucket(bucket5);

  AMDB_ASSERT_EQ(bucket1.start.StringValue(), histogram.GetBuckets()[0].start.StringValue());
  AMDB_ASSERT_EQ(bucket2.end.StringValue(), histogram.GetBuckets()[1].end.StringValue());
  AMDB_ASSERT_EQ(bucket3.count, histogram.GetBuckets()[2].count);
}

TEST_F(HistogramTest, Build) {
  expr::ExprValueLess less(expr::ExprValueCmp);
  std::map<expr::ExprValue, std::size_t, expr::ExprValueLess> map(less);
  map.insert(std::make_pair(expr::ExprValue::NewString("a"), 2));
  map.insert(std::make_pair(expr::ExprValue::NewString("b"), 11));
  map.insert(std::make_pair(expr::ExprValue::NewString("c"), 2));
  map.insert(std::make_pair(expr::ExprValue::NewString("d"), 7));
  map.insert(std::make_pair(expr::ExprValue::NewString("e"), 20));
  map.insert(std::make_pair(expr::ExprValue::NewString("f"), 15));

  Histogram histogram = BuildHistogram(map, 5);
  for (const auto& bucket : histogram.GetBuckets()) {
    std::cout << "start=" << bucket.start.StringValue() << ",end=" << bucket.end.StringValue()
              << ",count=" << bucket.count << std::endl;
  }
}
}  // namespace stat
}  // namespace amdb
