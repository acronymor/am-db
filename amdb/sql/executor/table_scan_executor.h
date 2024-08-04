#pragma once

#include "sql/chunk/chunk_util.h"
#include "sql/plan/range.h"
#include "sql/plan/table_scan.h"
#include "sql/scheduler/source.h"
#include "sql/storage/table_iterator.h"

namespace amdb {
namespace executor {
class TableScanExecutor : public scheduler::ISource {
 public:
  TableScanExecutor(StatementContext* ctx, plan::RelOptNode* plan)
      : ISource(ctx, scheduler::ExecutorType::kExecTableScan, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);

    scan_plan_ = dynamic_cast<plan::PhysicalTableScan*>(plan);
    primary_ranges_ = scan_plan_->PrimaryRanges();

    rel_table_ = scan_plan_->GetTable();
    table_ = ctx->arena->CreateObject<storage::Table>(ctx->arena, rel_table_->GetTable());
    row_desc_ = chunk::ToRowDesc(ctx_->arena, rel_table_->GetTable());
  };

  std::string Name() const override { return "TableScan"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Generate(chunk::Chunk* chunk) override;

 private:
  storage::Table* table_{nullptr};
  plan::RelOptTable* rel_table_{nullptr};
  storage::TableIterator* table_iter_{nullptr};
  std::vector<plan::IndexRange*>* primary_ranges_;

  plan::PhysicalTableScan* scan_plan_{nullptr};
  chunk::RowDescriptor* row_desc_{nullptr};
};
}  // namespace executor
}  // namespace amdb