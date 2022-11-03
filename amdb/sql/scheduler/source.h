#include "sql/chunk/chunk.h"
#include "sql/scheduler/executor.h"

namespace amdb {
namespace scheduler {
class ISource : public IExecutor {
 public:
  explicit ISource(StatementContext* ctx, ExecType type)
      : IExecutor(ctx, type) {
    chunk_ = ctx_->arena->CreateObject<chunk::Chunk>(ctx_->arena, ctx_->row_desc);
  };

  State Prepare() override;
  Status Work() override;

 protected:
  virtual Status Generate(chunk::Chunk* chunk) = 0;

 private:
  chunk::Chunk* chunk_;
};
}  // namespace scheduler
}  // namespace amdb