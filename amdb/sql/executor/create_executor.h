#include "sql/plan/create_database.h"
#include "sql/plan/create_table.h"
#include "sql/scheduler/executor.h"

namespace amdb {
namespace executor {
class CreateExecutor : public scheduler::IExecutor {
 public:
  CreateExecutor(StatementContext* ctx, scheduler::ExecutorType exec_type, plan::RelOptNode* plan)
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
  CreateDatabaseExecutor(StatementContext* ctx, plan::RelOptNode* plan)
      : CreateExecutor(ctx, scheduler::ExecutorType::kExecCreateDatabase, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(plan::RelOptNodeType::kPhysicalCreateDatabase, plan->GetType());
    create_database_plan_ = dynamic_cast<plan::PhysicalCreateDatabase*>(plan);
  };

 protected:
  Status DoWork() override;

 private:
  plan::PhysicalCreateDatabase* create_database_plan_{nullptr};
};

class CreateTableExecutor : public CreateExecutor {
 public:
  CreateTableExecutor(StatementContext* ctx, plan::RelOptNode* plan)
      : CreateExecutor(ctx, scheduler::ExecutorType::kExecCreateTable, plan) {
    AMDB_ASSERT_TRUE(plan != nullptr);
    AMDB_ASSERT_EQ(plan::RelOptNodeType::kPhysicalCreateTable, plan->GetType());
    create_table_plan_ = dynamic_cast<plan::PhysicalCreateTable*>(plan);
  };

 protected:
  Status DoWork() override;

 private:
  plan::PhysicalCreateTable* create_table_plan_{nullptr};
};
}  // namespace executor
}  // namespace amdb