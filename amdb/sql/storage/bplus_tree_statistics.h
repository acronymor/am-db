#pragma once

#include <iostream>
#include <string>

namespace amdb {
namespace storage {
struct NodeStatistics {
  // max_key of the subtree
  std::string max_key;
  // min_key of the subtree
  std::string min_key;
  // total count of the subtree
  uint64_t count = 0;
  // total size in byte of the node
  uint64_t node_size = 0;
};
}  // namespace storage
}  // namespace amdb