#pragma once

#include <iostream>
#include <limits>

namespace amdb {
const constexpr uint32_t kInvalidTupleID = std::numeric_limits<uint32_t>::max();
const constexpr uint64_t kInvalidColumnID =
    std::numeric_limits<uint64_t>::max();
const constexpr uint64_t kInvalidIDatabaseID =
    std::numeric_limits<uint64_t>::max();
const constexpr uint64_t kInvalidTableID = std::numeric_limits<uint64_t>::max();
const constexpr uint32_t kInvalidIndexID = std::numeric_limits<uint32_t>::max();
}  // namespace amdb