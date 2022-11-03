#include "sql/storage/iterator.h"
#include "sql/storage/table.h"

namespace amdb {
namespace storage {
class TableIterator : public BaseIterator {
 public:
  TableIterator(Table* table, uint64_t page_size)
      : BaseIterator(table->row_index, page_size){};

  Status GetItem(chunk::Chunk* chunk) override;
};
}  // namespace storage
}  // namespace amdb