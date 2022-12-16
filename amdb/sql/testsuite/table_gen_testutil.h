#include "sql/chunk/chunk.h"
#include "sql/storage/table.h"
#include "sql/testsuite/data_gen_testutil.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace testsuite {
class TableGen : public SchemaGen {
 public:
  TableGen() : SchemaGen() { data_gen_ = std::make_unique<SeqDataGen>(arena_, 0); };

  storage::Table* GetTable(uint64_t db_id, uint64_t table_id);

  chunk::Chunk* GenRows(schema::TableInfo* table_info, size_t size);

 private:
  expr::ExprValue toExprValue(schema::ColumnType type);

  std::unique_ptr<SeqDataGen> data_gen_{nullptr};
  storage::Table* table_{nullptr};

  std::unordered_map<std::string, storage::Table*> tables_;
};
}  // namespace testsuite
}  // namespace amdb