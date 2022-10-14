#pragma once

#include "sql/chunk/chunk.h"
#include "sql/common/statuscode.h"
#include "sql/storage/bplus_tree_iterator.h"
#include "sql/storage/kv_storage_util.h"

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
  BaseIterator(Bptree* bptree, TreeCtx* tree_ctx, uint64_t page_size);

  virtual ~BaseIterator() override = default;

  Status Open() override;

  inline bool HasNext() override { return true; }

  void Next() override;

  inline bool Valid() override { return code_ == Status::C_OK; }

  inline Status Code() override { return code_; }

 protected:
  std::vector<std::pair<std::string, std::string>> data_segment_;
  size_t visited_kv_count_ = 0;
  TreeCtx* tree_ctx_;
  std::vector<IteratorOptions> it_ops_;
  uint64_t current_op_idx_ = 0;

 private:
  Status code_ = Status::C_OK;

  Bptree* bptree_;
  BptIterator* tree_it_{nullptr};

  uint64_t page_size_;
};

class IndexIterator : public BaseIterator {
 public:
  IndexIterator(Bptree* bptree, TreeCtx* tree_ctx, const uint64_t page_size)
      : BaseIterator(bptree, tree_ctx, page_size){};

  Status InitIterOptions(const IteratorOptions& it_opts);

  Status GetItem(chunk::Chunk* chunk) override;
};

class TableIterator : public BaseIterator {
 public:
  TableIterator(Bptree* bptree, TreeCtx* tree_ctx, const uint64_t page_size)
      : BaseIterator(bptree, tree_ctx, page_size) {
    kv_util =
        tree_ctx_->AllocMem()->CreateObject<KvStorageUtil>(tree_ctx_->KvApi());
  };

  Status GetItem(chunk::Chunk* chunk) override;

 private:
  KvStorageUtil* kv_util;
};
}  // namespace storage
}  // namespace amdb
