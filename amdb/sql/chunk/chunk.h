#include <iostream>
#include <vector>

#include "sql/common/statuscode.h"

namespace amdb {
namespace chunk {
class Chunk {
 public:
  ~Chunk() = default;

  Chunk() = default;

  Chunk& operator=(Chunk&& rsh) noexcept;

  Chunk(const Chunk& c) = delete;

  Chunk& operator=(const Chunk& c) = delete;

  Status PullIndexData(std::vector<std::pair<std::string, std::string>>& data_segment);

  Status PullKvData(std::vector<std::pair<std::string, std::string>>& data_segment);

};
}  // namespace chunk
}  // namespace amdb