#pragma once

#include <iostream>
#include <vector>

#include "sql/common/statuscode.h"

namespace amdb {
namespace chunk {
class Chunk {
 public:
  friend class Iterator;
  ~Chunk() = default;

  Chunk() = default;

  Chunk& operator=(Chunk&& rsh) noexcept;

  Chunk(const Chunk& c) = delete;

  Chunk& operator=(const Chunk& c) = delete;

  uint32_t Size() const;

  Status PullIndexData(
      std::vector<std::pair<std::string, std::string>>& data_segment);

  Status PullKvData(
      std::vector<std::pair<std::string, std::string>>& data_segment);

 private:
  std::vector<uint32_t> select_;
};
}  // namespace chunk
}  // namespace amdb