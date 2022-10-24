#include "sql/chunk/iterator.h"

#include "row.h"

namespace amdb {
namespace chunk {

void Iterator::Open() { cursor_ = 0; }

bool Iterator::HasNext() { return cursor_ < chunk_->select_.size(); }

void Iterator::Next() { cursor_++; }

chunk::Row* Iterator::GetRow() {
  return chunk_->select_.at(cursor_);
}

}  // namespace chunk
}  // namespace amdb