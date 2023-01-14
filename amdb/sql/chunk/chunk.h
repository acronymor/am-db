#pragma once

#include <iostream>
#include <vector>

#include "sql/chunk/row.h"
#include "sql/common/statuscode.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace chunk {
class Chunk {
 public:
  ~Chunk() = default;
  Chunk() = default;

  Chunk(const Chunk& c) = delete;
  Chunk& operator=(const Chunk& c) = delete;

  Chunk(Arena* arena);

  Chunk& operator=(Chunk&& chunk) noexcept;

  uint32_t Size() const;

  Status PullData(schema::TableInfo* table_info, std::vector<std::string>& values);

  void AddRow(Row* row);

  void ResetCursor() { cursor_ = 0; }
  void Next() { ++cursor_; }
  bool ReachEnd() { return cursor_ >= select_.size(); }
  Row* CurrentRow() { return select_.at(cursor_); }

  uint32_t cursor_ = 0;
  std::vector<Row*> select_;

  void SetRowDesc(RowDescriptor* row_desc);
  RowDescriptor* GetRowDesc();
#ifdef AMDB_BUILD_TEST
 public:
#else
 private:
#endif
  RowDescriptor* row_desc_{nullptr};

  uint32_t cur_capacity_ = 0;
  uint32_t max_capacity_ = 1024;

  Arena* arena_;
};
}  // namespace chunk
}  // namespace amdb