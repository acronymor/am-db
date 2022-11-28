#include "sql/scheduler/sink.h"

namespace amdb {
namespace executor {
class ResultSetExecutor : public scheduler::ISink {
 public:
  ResultSetExecutor(StatementContext* ctx, Type exec_type,
                    planner::PhysicalNode* plan)
      : ISink(ctx, exec_type, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(planner::PhysicalNode::Type::kPhysicalResultSet, plan->type);
    result_plan_ = static_cast<planner::PhysicalResultSet*>(plan);
  };

  std::string Name() const override { return "ResultSet"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Consume(chunk::Chunk* chunk) override;

 private:
  planner::PhysicalResultSet* result_plan_{nullptr};
};
}  // namespace executor
}  // namespace amdb