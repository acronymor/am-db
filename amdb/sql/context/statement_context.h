#include "sql/common/arena.h"
#include "sql/chunk/row_description.h"

namespace amdb {
class StatementContext {
 public:
  Arena* arena;

  chunk::RowDescriptor* row_desc;
};
}  // namespace amdb