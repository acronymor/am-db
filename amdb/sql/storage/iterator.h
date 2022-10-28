#pragma once

#include "sql/storage/bplus_tree_iterator.h"
#include "sql/chunk/chunk.h"
#include "sql/common/statuscode.h"
#include "sql/schema/schema.h"
#include "sql/storage/index.h"

namespace amdb {
namespace storage {

class Iterator {
 public:
  virtual ~Iterator() = default;

  [[nodiscard]] virtual Status Open() = 0;

  virtual bool HasNext() = 0;

  [[nodiscard]] virtual Status GetItem(chunk::Chunk* chunk) = 0;

  virtual void Next() = 0;

  virtual bool Valid() = 0;

  virtual Status Code() = 0;
};

class BaseIterator : public Iterator {
 public:
  BaseIterator(Index* index, uint64_t page_size);

  virtual ~BaseIterator() override = default;

  Status Open() override;

  inline bool HasNext() override { return true; }

  void Next() override;

  inline bool Valid() override { return code_ == Status::C_OK; }

  inline Status Code() override { return code_; }

  Status InitIterOptions(const IteratorOptions& it_opts);

 protected:
  std::vector<std::pair<std::string, std::string>> data_segment_;
  size_t visited_kv_count_ = 0;
  std::vector<IteratorOptions> it_ops_;
  uint64_t current_op_idx_ = 0;

 private:
  Status code_ = Status::C_OK;

  Index* index_;
  BptIterator* tree_it_{nullptr};

  uint64_t page_size_;
};
}  // namespace storage
}  // namespace amdb