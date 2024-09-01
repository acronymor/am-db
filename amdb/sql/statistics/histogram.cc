#include "sql/statistics/histogram.h"

#include <numeric>

#include "sql/codec/codec.h"

namespace amdb {
namespace stat {
Histogram BuildHistogram(const std::map<expr::ExprValue, std::size_t, expr::ExprValueLess>& value_map,
                         const std::size_t bucket_size) {
  Histogram histogram(bucket_size);
  const std::size_t total_count = std::accumulate(
      std::begin(value_map), std::end(value_map), 0,
      [](const std::size_t previous, const auto& element) -> std::size_t { return previous + element.second; });
  const double avg_bucket_size = total_count / static_cast<double>(bucket_size);

  std::size_t ndv = 0;
  std::size_t sum = 0;
  std::size_t cumulative_sum = 0;
  auto freq_it = value_map.begin();
  auto* lowest_value = &freq_it->first;
  double current_threshold = avg_bucket_size;

  for (; freq_it != value_map.end(); ++freq_it) {
    ndv++;
    sum += freq_it->second;
    cumulative_sum += freq_it->second;
    auto next = std::next(freq_it);
    if (next != value_map.end()) {
      const std::size_t predict_cumulative_sum = cumulative_sum + next->second;
      if (predict_cumulative_sum <= current_threshold) {
        continue;
      }

      if (std::abs(current_threshold - cumulative_sum) >= std::abs(current_threshold - predict_cumulative_sum)) {
        continue;
      }
    }

    double cumulative_frequency = cumulative_sum / static_cast<double>(total_count);
    const Histogram::Bucket bucket = {
        .start = *lowest_value, .end = freq_it->first, .count = sum, .ndv = ndv, .cf = cumulative_frequency};
    histogram.AddBucket(bucket);

    sum = 0;
    ndv = 0;
    current_threshold = avg_bucket_size * (histogram.GetBuckets().size() + 1);

    if (next != value_map.end()) {
      lowest_value = &next->first;
    }
  }
  return histogram;
}

Status Histogram::Bucket::Serialize(BucketProto* output) {
  auto to_string = [](const expr::ExprValue& value) -> std::string {
    std::string s;
    codec::EncodeExprValue(value, &s);
    return s;
  };

  output->set_start(std::move(to_string(this->start)));
  output->set_end(std::move(to_string(this->end)));
  output->set_count(this->count);
  output->set_ndv(this->ndv);
  output->set_cf(this->cf);
  return Status::C_OK;
}

Status Histogram::Bucket::Deserialize(const BucketProto& input) {
  auto to_expr_value = [](const std::string& s) -> expr::ExprValue {
    expr::ExprValue value = expr::ExprValue::NewEmpty(expr::String);
    codec::DecodeExprValue(s, &value);
    return value;
  };

  this->start = to_expr_value(input.start());
  this->end = to_expr_value(input.end());
  this->count = input.count();
  this->ndv = input.ndv();
  this->cf = input.cf();
  return Status::C_OK;
}

Histogram::Histogram(std::size_t bucket_num) { buckets_.reserve(bucket_num); }

Status Histogram::AddBucket(const Bucket& bucket) {
  this->buckets_.emplace_back(std::move(bucket));
  return Status::C_OK;
}

}  // namespace stat
}  // namespace amdb