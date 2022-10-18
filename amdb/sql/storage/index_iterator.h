#include "sql/storage/iterator.h"

namespace amdb {
namespace storage {
class IndexIterator : public BaseIterator {
 public:
  IndexIterator(Bptree* bptree, TreeCtx* tree_ctx, const uint64_t page_size)
      : BaseIterator(bptree, tree_ctx, page_size){};

  Status InitIterOptions(const IteratorOptions& it_opts);

  Status GetItem(chunk::Chunk* chunk) override;
};
}  // namespace storage
}  // namespace amdb