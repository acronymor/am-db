#include "sql/chunk/iterator.h"

#include "sql/row/row.h"

namespace amdb {
namespace chunk {

void Iterator::Open() { cursor_ = 0; }

bool Iterator::HasNext() { return cursor_ < chunk_->select_.size(); }

void Iterator::Next() { cursor_++; }

row::Row* Iterator::GetRow() {
  chunk_->select_.at(cursor_);
  // TODO get row
  return nullptr;
}

}  // namespace chunk
}  // namespace amdb