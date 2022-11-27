#include "sql/scheduler/source.h"
#include "sql/storage/table_iterator.h"

namespace amdb {
namespace executor {
class TableScanExecutor : public scheduler::ISource {
 public:
  TableScanExecutor(StatementContext* ctx, Type exec_type,
                    schema::TableInfo* table_info)
      : table_info_(table_info), ISource(ctx, exec_type) {
    table_ = ctx->arena->CreateObject<storage::Table>(ctx->arena, table_info_);

    // TODO extract primary_range from physical_node
    planner::IndexRange* ir = ctx->arena->CreateObject<planner::IndexRange>();
    ir->lower.value.push_back(expr::ExprValue::NewString("", ctx->arena));
    ir->lower.type = planner::IndexRange::RangePointType::LEFT_CLOSE;
    ir->lower_str = "";
    ir->upper.value.push_back(expr::ExprValue::NewUInt8(3));
    ir->upper.value.push_back(expr::ExprValue::NewString("c", ctx->arena));
    ir->upper.type = planner::IndexRange::RangePointType::RIGHT_OPEN;
    ir->upper_str = "c";
    primary_ranges_.push_back(ir);
  };

  std::string Name() const override {
      return "TableScan";
  };

  Status Open() override;
  Status Close() override;

 protected:
  Status Generate(chunk::Chunk* chunk) override;

 private:
  storage::Table* table_{nullptr};
  schema::TableInfo* table_info_{nullptr};
  storage::TableIterator* table_iter_{nullptr};
  std::vector<planner::IndexRange*> primary_ranges_;
};
}  // namespace executor
}  // namespace amdb