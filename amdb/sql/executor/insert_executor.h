#pragma once

#include "sql/chunk/chunk_util.h"
#include "sql/scheduler/sink.h"
#include "sql/storage/table.h"

namespace amdb {
namespace executor {
class InsertExecutor : public scheduler::ISink {
 public:
  InsertExecutor(StatementContext* ctx, Type exec_type, planner::PhysicalNode* plan) : ISink(ctx, exec_type, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(planner::PhysicalNode::Type::kPhysicalInsert, plan->type);
    insert_plan_ = dynamic_cast<planner::PhysicalInsert*>(plan);
    table_ = ctx->arena->CreateObject<storage::Table>(ctx->arena, insert_plan_->table_info);
    row_desc_ = chunk::ToRowDesc(ctx_->arena, insert_plan_->table_info);
  };

  std::string Name() const override { return "Insert"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Consume(chunk::Chunk* chunk) override;

 private:
  planner::PhysicalInsert* insert_plan_{nullptr};
  storage::Table* table_{nullptr};
  chunk::RowDescriptor* row_desc_{nullptr};
};
}  // namespace executor
}  // namespace amdb