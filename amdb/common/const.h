#pragma once

#include <iostream>
#include <limits>

namespace amdb {
constexpr uint64_t kInvalidIDatabaseID = std::numeric_limits<uint64_t>::max();
constexpr uint64_t kInvalidTableID = std::numeric_limits<uint64_t>::max();
constexpr uint32_t kInvalidIndexID = std::numeric_limits<uint32_t>::max();
constexpr uint64_t kInvalidColumnID = std::numeric_limits<uint64_t>::max();
}  // namespace amdb