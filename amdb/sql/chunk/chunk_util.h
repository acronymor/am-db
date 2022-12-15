#include "sql/chunk/row_description.h"
#include "sql/common/arena.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace chunk {
chunk::RowDescriptor* ToRowDesc(Arena* arena, schema::TableInfo* table_info);
}  // namespace chunk
}  // namespace amdb