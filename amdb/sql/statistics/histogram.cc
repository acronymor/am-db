#include "sql/statistics/histogram.h"

#include "sql/codec/codec.h"

namespace amdb {
namespace stat {

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