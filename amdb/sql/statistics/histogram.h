#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "proto/statistics.pb.h"
#include "sql/common/statuscode.h"
#include "sql/expression/expr_cmp.h"
#include "sql/expression/expr_value.h"

namespace amdb {
namespace stat {
class Histogram {
 public:
  struct Bucket {
    expr::ExprValue start;
    expr::ExprValue end;
    std::uint64_t count;
    std::uint64_t ndv;
    std::double_t cf;  // cumulative frequency

    // serialize to proto
    Status Serialize(BucketProto* output);
    // deserialize from proto
    Status Deserialize(const BucketProto& input);
  };

  Histogram() = default;
  explicit Histogram(std::size_t bucket_num);

  Status AddBucket(const Bucket& bucket);

  inline const std::vector<Bucket>& GetBuckets() const { return this->buckets_; }

 private:
  std::vector<Bucket> buckets_;
};

Histogram BuildHistogram(const std::map<expr::ExprValue, std::size_t, expr::ExprValueLess>& value_map, const std::size_t bucket_size);
}  // namespace stat
}  // namespace amdb