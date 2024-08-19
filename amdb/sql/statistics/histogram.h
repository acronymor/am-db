#include <cstdint>
#include <string>
#include <vector>

#include "proto/statistics.pb.h"
#include "sql/common/statuscode.h"

namespace amdb {
namespace stat {
class Histogram {
 public:
  struct Bucket {
    std::string start;
    std::string end;
    std::uint64_t count;
    std::uint64_t ndv;
    std::uint64_t cf;  // cumulative frequency

    // serialize to proto
    Status Serialize(BucketProto* output);
    // deserialize from proto
    Status Deserialize(const BucketProto& input);
  };

  Histogram() { this->buckets_.reserve(256); };
  Histogram(double sample_rate, std::size_t bucket_num);

  Status AddBucket(const Bucket& bucket);

  inline const std::vector<Bucket>& GetBuckets() const { return this->buckets_; }

 private:
  std::vector<Bucket> buckets_;
};
}  // namespace stat
}  // namespace amdb