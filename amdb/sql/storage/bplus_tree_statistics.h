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
  /*
  // total size in byte of the node
  uint64_t data_size = 0;
   */
};

struct IncrStatistics {
  int64_t node_size = 0;
  int64_t data_size = 0;

  void Add(const IncrStatistics& stat) {
    node_size += stat.node_size;
    data_size += stat.data_size;
  }
};
}  // namespace storage
}  // namespace amdb