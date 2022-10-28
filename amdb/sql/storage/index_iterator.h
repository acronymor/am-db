#include "sql/storage/iterator.h"

namespace amdb {
namespace storage {
class IndexIterator : public BaseIterator {
 public:
  IndexIterator(Index* index, const uint64_t page_size)
      : BaseIterator(index, page_size){};

  Status GetItem(chunk::Chunk* chunk) override;
};
}  // namespace storage
}  // namespace amdb