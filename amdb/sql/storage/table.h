#include <unordered_map>

#include "sql/common/statuscode.h"
#include "sql/schema/schema.h"
#include "sql/storage/index.h"
#include "sql/storage/iterator.h"

namespace amdb {
namespace storage {
class Table {
 public:
  ~Table() = default;

  Table(Arena* arena, schema::TableInfo* table_info, KvStorageAPI* api);

  Status Prepare();

  Status Submit();

  Status Insert(chunk::Chunk* chunk);

  Status Delete(chunk::Chunk* chunk);

  Status Update(chunk::Chunk* old_chunk, chunk::Chunk* new_chunk);

 private:
  Status insertCore(chunk::Chunk* chunk);

  Status deleteCore(chunk::Chunk* chunk);

  Status updateCore(chunk::Chunk* old_chunk, chunk::Chunk* new_chunk);

  schema::TableInfo* table_info_{nullptr};

  Index* row_index_{nullptr};
  std::unordered_map<uint64_t, Index*> col_index_;

  TreeCtx* tree_ctx_;
  KvStorageAPI* kv_api_;
};
}  // namespace storage
}  // namespace amdb