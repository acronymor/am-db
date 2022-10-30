#include "sql/chunk/chunk.h"
#include "sql/common/statuscode.h"
#include "sql/schema/schema.h"
#include "sql/storage/bplus_tree.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class Index {
 public:
  ~Index() = default;

  Index(KvStorageAPI* api, Arena* arena, schema::TableInfo* table_info,
        schema::IndexInfo* index_info, BptNonLeafNodeProto* root);

  Status Save();

  Status Insert(chunk::Chunk* chunk);

  Status Delete(chunk::Chunk* chunk);

  Bptree* Tree();

  TreeCtx* TreeCtxx();

#ifdef AMDB_BUILD_TEST
 public:
#else
 private:
#endif
  schema::TableInfo* table_info_{nullptr};
  schema::IndexInfo* index_info_{nullptr};

  Bptree* bptree_;
  TreeCtx* tree_ctx_;

  KvStorageAPI* kv_api_;
};
}  // namespace storage
}  // namespace amdb