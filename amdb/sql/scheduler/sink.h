#pragma once

#include "sql/chunk/chunk.h"
#include "sql/scheduler/executor.h"

namespace amdb {
namespace scheduler {
class ISink : public IExecutor {
 public:
  explicit ISink(StatementContext* ctx, ExecutorType exec_type, plan::RelOptNode* plan)
      : IExecutor(ctx, exec_type, reinterpret_cast<plan::RelOptNode*>(plan)) {
    chunk_ = ctx_->arena->CreateObject<chunk::Chunk>(ctx_->arena);
  };

 public:
  State Prepare() override;
  Status Work() override;

 protected:
  virtual Status Consume(chunk::Chunk* chunk) = 0;

 private:
  chunk::Chunk* chunk_;
};
}  // namespace scheduler
}  // namespace amdb