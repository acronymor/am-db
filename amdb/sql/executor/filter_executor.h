#include "sql/scheduler/transform.h"

namespace amdb {
namespace executor {
class FilterExecutor : public scheduler::ITransform {
 public:
  FilterExecutor(StatementContext* ctx, Type exec_type,
                 planner::PhysicalNode* plan)
      : ITransform(ctx, exec_type, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(planner::PhysicalNode::Type::kPhysicalFilter, plan->type);
    filter_plan_ = static_cast<planner::PhysicalFilter*>(plan);
  };

  std::string Name() const override { return "Filter"; };

  Status Open() override;
  Status Close() override;

 protected:
  Status Transform(chunk::Chunk* input_chunk,
                   chunk::Chunk* output_chunk) override;

 private:
  planner::PhysicalFilter* filter_plan_{nullptr};
};
}  // namespace executor
}  // namespace amdb