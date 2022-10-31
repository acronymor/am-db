#include <string>
#include <unordered_map>

#include "sql/chunk/chunk.h"
#include "sql/chunk/row_description.h"
#include "sql/common/arena.h"
#include "sql/schema/schema.h"
#include "sql/testsuite/kv_data_testutil.h"

namespace amdb {
namespace testsuite {
class SchemaGen : public KvDataTest {
 protected:
  void SetUp() override;

  void TearDown() override;

 public:
  schema::TableInfo* GenTableInfo(uint64_t db_id, uint64_t table_id);

  chunk::RowDescriptor* GenRowDesc();

  chunk::Chunk* GenRows(chunk::RowDescriptor* row_desc, size_t size);

 private:
  schema::TableInfo* table_info_{nullptr};
};
}  // namespace testsuite
}  // namespace amdb