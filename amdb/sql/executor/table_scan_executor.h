#pragma once

#include "sql/scheduler/source.h"
#include "sql/storage/table_iterator.h"

namespace amdb {
namespace executor {
class TableScanExecutor : public scheduler::ISource {
 public:
  TableScanExecutor(StatementContext* ctx, Type exec_type,
                    planner::PhysicalNode* plan)
      :ISource(ctx, exec_type, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(planner::PhysicalNode::Type::kPhysicalTableScan, plan->type);
    scan_plan_ = dynamic_cast<planner::PhysicalTableScan*>(plan);
    table_info_ = scan_plan_->table_info;
    table_ = ctx->arena->CreateObject<storage::Table>(ctx->arena, table_info_);
    primary_ranges_ = scan_plan_->primary_ranges;
  };

  std::string Name() const override { return "TableScan"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Generate(chunk::Chunk* chunk) override;

 private:
  storage::Table* table_{nullptr};
  schema::TableInfo* table_info_{nullptr};
  storage::TableIterator* table_iter_{nullptr};
  std::vector<planner::IndexRange*> primary_ranges_;

  planner::PhysicalTableScan* scan_plan_{nullptr};
};
}  // namespace executor
}  // namespace amdb