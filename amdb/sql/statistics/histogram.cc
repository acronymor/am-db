#include "sql/statistics/histogram.h"

namespace amdb {
namespace stat {

Status Histogram::Bucket::Serialize(BucketProto* output) {
  output->set_start(this->start);
  output->set_end(this->end);
  output->set_count(this->count);
  output->set_ndv(this->ndv);
  output->set_cf(this->cf);
  return Status::C_OK;
}

Status Histogram::Bucket::Deserialize(const BucketProto& input) {
  this->start = input.start();
  this->end = input.end();
  this->count = input.count();
  this->ndv = input.ndv();
  this->cf = input.cf();
  return Status::C_OK;
}

Histogram::Histogram(double sample_rate, std::size_t bucket_num) { buckets_.reserve(bucket_num); }

Status Histogram::AddBucket(const Bucket& bucket) {
  this->buckets_.emplace_back(std::move(bucket));
  return Status::C_OK;
}

}  // namespace stat
}  // namespace amdb