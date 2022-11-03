#include "sql/chunk/chunk.h"
#include "sql/scheduler/executor.h"

namespace amdb {
namespace scheduler {
class ISink : public IExecutor {
 public:
  explicit ISink(StatementContext* ctx, ExecType exec_type)
      : IExecutor(ctx, exec_type) {
    chunk_ =
        ctx_->arena->CreateObject<chunk::Chunk>(ctx_->arena, ctx_->row_desc);
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