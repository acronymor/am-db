#include "sql/chunk/row_description.h"
#include "sql/common/arena.h"

namespace amdb {
class StatementContext {
 public:
  Arena* arena;

  chunk::RowDescriptor* row_desc;
};
}  // namespace amdb