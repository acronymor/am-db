#include "sql/common/statuscode.h"
#include "sql/storage/iterator.h"

namespace amdb {
namespace storage {
class Index {
 public:
  ~Index() = default;

  Index(BptNonLeafNodeProto *root, KvStorageAPI *api, Arena *arena);

  Status Save();

  Status Insert(chunk::Chunk* chunk);

  Status Delete(chunk::Chunk* chunk);

 private:
  Bptree* bptree_;
  TreeCtx* tree_ctx_;
};
}  // namespace storage
}  // namespace amdb
