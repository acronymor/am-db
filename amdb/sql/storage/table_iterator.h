#include "sql/storage/iterator.h"

namespace amdb {
namespace storage {
class TableIterator : public BaseIterator {
 public:
  TableIterator(Bptree* bptree, TreeCtx* tree_ctx, const uint64_t page_size)
      : BaseIterator(bptree, tree_ctx, page_size){};

  Status GetItem(chunk::Chunk* chunk) override;
};
}  // namespace storage
}  // namespace amdb