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

  Chunk(const Chunk& c) = delete;
  Chunk(Arena* arena, RowDescriptor* row_desc);

  Chunk& operator=(Chunk&& rsh) noexcept;
  friend bool operator==(const Chunk& lhs, const Chunk& rhs);
  Chunk& operator=(const Chunk& c) = delete;

  uint32_t Size() const;

  Status PullIndexData(std::vector<std::pair<std::string, std::string>>& data_segment);
  Status PullKvData(std::vector<std::pair<std::string, std::string>>& data_segment);

  void AddRow(Row* row);

  void ResetCursor() { cursor_ = 0; }
  void Next() { ++cursor_; }
  bool ReachEnd() { return cursor_ >= select_.size(); }
  Row* CurrentRow() { return select_.at(cursor_); }

#ifdef AMDB_BUILD_TEST
 public:
#else
 private:
#endif

  uint32_t cursor_ = 0;
  std::vector<Row*> select_;

  RowDescriptor* row_desc_{nullptr};
  std::vector<Column*> columns_;

  uint32_t cur_capacity_ = 0;
  uint32_t max_capacity_ = 1024;

  Arena* arena_;
};
}  // namespace chunk
}  // namespace amdb