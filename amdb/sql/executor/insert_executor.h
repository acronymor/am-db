#pragma once

#include "sql/chunk/chunk_util.h"
#include "sql/plan/insert.h"
#include "sql/scheduler/sink.h"
#include "sql/storage/table.h"

namespace amdb {
namespace executor {
class InsertExecutor : public scheduler::ISink {
 public:
  InsertExecutor(StatementContext* ctx, scheduler::ExecutorType exec_type, plan::RelOptNode* plan)
      : ISink(ctx, exec_type, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(plan::RelOptNodeType::kPhysicalInsert, plan->GetType());
    insert_plan_ = dynamic_cast<plan::PhysicalInsert*>(plan);
    table_ = ctx->arena->CreateObject<storage::Table>(ctx->arena, insert_plan_->GetTable()->GetTable());
    row_desc_ = chunk::ToRowDesc(ctx_->arena, insert_plan_->GetTable()->GetTable());
  };

  std::string Name() const override { return "Insert"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Consume(chunk::Chunk* chunk) override;

 private:
  plan::PhysicalInsert* insert_plan_{nullptr};
  storage::Table* table_{nullptr};
  chunk::RowDescriptor* row_desc_{nullptr};
};
}  // namespace executor
}  // namespace amdb