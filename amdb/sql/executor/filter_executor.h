#pragma once

#include "sql/chunk/chunk_util.h"
#include "sql/plan/filter.h"
#include "sql/scheduler/transform.h"

namespace amdb {
namespace executor {
class FilterExecutor : public scheduler::ITransform {
 public:
  FilterExecutor(StatementContext* ctx, plan::RelOptNode* plan)
      : ITransform(ctx, scheduler::ExecutorType::kExecFilter, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(plan::RelOptNodeType::kPhysicalFilter, plan->GetType());
    filter_plan_ = dynamic_cast<plan::PhysicalFilter*>(plan);
  };

  std::string Name() const override { return "Filter"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Transform(chunk::Chunk* input_chunk, chunk::Chunk* output_chunk) override;

 private:
  plan::PhysicalFilter* filter_plan_{nullptr};
};
}  // namespace executor
}  // namespace amdb