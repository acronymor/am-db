#pragma once

#include <cstdint>

namespace amdb {
namespace codec {

enum struct DataType : uint8_t {
  kTreeNode = 0,
  kRow = 1,
  kDatabaseId = 2,
  kTableId = 3,
  kColumnId = 4,
  kTableStatistic = 5,
  kColumnStatistic = 6
};

}
}  // namespace amdb