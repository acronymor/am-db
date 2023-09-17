#pragma once

#include "sql/plan/result_set.h"
#include "sql/scheduler/sink.h"

namespace amdb {
namespace executor {
class ResultSetExecutor : public scheduler::ISink {
 public:
  ResultSetExecutor(StatementContext* ctx, plan::RelOptNode* plan)
      : ISink(ctx, scheduler::ExecutorType::kExecResultSet, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(plan::RelOptNodeType::kPhysicalResultSet, plan->GetType());
    result_plan_ = dynamic_cast<plan::PhysicalResultSet*>(plan);
  };

  std::string Name() const override { return "ResultSet"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Consume(chunk::Chunk* chunk) override;

 private:
  plan::PhysicalResultSet* result_plan_{nullptr};
};
}  // namespace executor
}  // namespace amdb