#pragma once

#include "sql/chunk/chunk.h"
#include "sql/chunk/row.h"
#include "sql/common/statuscode.h"

namespace amdb {
namespace chunk {
class Iterator {
 public:
  Iterator(Chunk* chunk) : chunk_{chunk} {};

  ~Iterator() = default;

  void Open();

  bool HasNext();

  void Next();

  chunk::Row* GetRow();

 private:
  Chunk* chunk_ = nullptr;
  size_t cursor_ = 0;
};
}  // namespace chunk
}  // namespace amdb