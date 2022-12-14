#pragma once

#include <unordered_map>

#include "sql/common/statuscode.h"
#include "sql/schema/schema.h"
#include "sql/storage/iterator.h"
#include "sql/storage/metadata.h"

namespace amdb {
namespace storage {
class Table {
 public:
  ~Table() = default;

  Table(Arena* arena, schema::TableInfo* table_info);

  Status Prepare();

  Status Submit();

  Status Insert(chunk::Chunk* chunk);

  Status Delete(chunk::Chunk* chunk);

  Status Update(chunk::Chunk* old_chunk, chunk::Chunk* new_chunk);

 public:
  Index* row_index{nullptr};
  std::unordered_map<uint64_t, Index*> col_index;

 private:
  Status insertCore(chunk::Chunk* chunk);

  Status deleteCore(chunk::Chunk* chunk);

  Status updateCore(chunk::Chunk* old_chunk, chunk::Chunk* new_chunk);

  Status loadMeta();

#ifdef AMDB_BUILD_TEST
 public:
#endif
  schema::TableInfo* table_info_{nullptr};
  Arena* arena_;

  Metadata* metadata_api_;
  KvStorageAPI* kv_api_;
};
}  // namespace storage
}  // namespace amdb