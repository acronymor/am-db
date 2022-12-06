#include "sql/scheduler/executor.h"

namespace amdb {
namespace executor {
class CreateExecutor : public scheduler::IExecutor {
 public:
  CreateExecutor(StatementContext* ctx, Type exec_type, planner::PhysicalNode* plan)
      : IExecutor(ctx, exec_type, plan){};
  ~CreateExecutor() override = default;
  std::string Name() const override { return "Create"; };

  Status Open() override;
  Status Close() override;

  State Prepare() override;
  Status Work() override;

 protected:
  virtual Status DoWork() = 0;
};

class CreateDatabaseExecutor : public CreateExecutor {
 public:
  CreateDatabaseExecutor(StatementContext* ctx, Type exec_type, planner::PhysicalNode* plan)
      : CreateExecutor(ctx, exec_type, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(planner::PhysicalNode::Type::kPhysicalCreateDatabase, plan->type);
    create_database_plan_ = dynamic_cast<planner::PhysicalCreateDatabase*>(plan);
  };

 protected:
  Status DoWork() override;

 private:
  planner::PhysicalCreateDatabase* create_database_plan_{nullptr};
};

class CreateTableExecutor : public CreateExecutor {
 public:
  CreateTableExecutor(StatementContext* ctx, Type exec_type, planner::PhysicalNode* plan)
      : CreateExecutor(ctx, exec_type, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(planner::PhysicalNode::Type::kPhysicalCreateTable, plan->type);
    create_table_plan_ = dynamic_cast<planner::PhysicalCreateTable*>(plan);
  };

 protected:
  Status DoWork() override;

 private:
  planner::PhysicalCreateTable* create_table_plan_{nullptr};
};
}  // namespace executor
}  // namespace amdb