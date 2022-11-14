#include "sql/scheduler/source.h"
#include "sql/storage/table_iterator.h"

namespace amdb {
namespace executor {
class TableScanExecutor : public scheduler::ISource {
 public:
  TableScanExecutor(StatementContext* ctx, ExecType exec_type,
                    schema::TableInfo* table_info)
      : table_info_(table_info), ISource(ctx, exec_type) {
    table_ = ctx->arena->CreateObject<storage::Table>(ctx->arena, table_info_);
  };

  Status Open() override;
  Status Close() override;

 protected:
  Status Generate(chunk::Chunk* chunk) override;

 private:
  storage::Table* table_{nullptr};
  schema::TableInfo* table_info_{nullptr};
  storage::TableIterator* table_iter_{nullptr};
};
}  // namespace executor
}  // namespace amdb