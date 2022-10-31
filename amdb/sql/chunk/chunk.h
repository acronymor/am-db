#pragma once

#include <iostream>
#include <vector>

#include "sql/chunk/row.h"
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

  Chunk(Arena* arena, RowDescriptor* row_desc);

  uint32_t Size() const;

  Status PullIndexData(
      std::vector<std::pair<std::string, std::string>>& data_segment);

  Status PullKvData(
      std::vector<std::pair<std::string, std::string>>& data_segment);

  void AddRow(Row* row);

#ifdef AMDB_BUILD_TEST
 public:
#else
 private:
#endif

  std::vector<Row*> select_;

  RowDescriptor* row_desc_{nullptr};
  std::vector<Column*> columns_;

  uint32_t cur_capacity_ = 0;

  Arena* arena_;
};
}  // namespace chunk
}  // namespace amdb