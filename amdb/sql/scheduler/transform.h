#include "sql/chunk/chunk.h"
#include "sql/scheduler/executor.h"

namespace amdb {
namespace scheduler {
class ITransform : public IExecutor {
 public:
  explicit ITransform(StatementContext* ctx, Type exec_type, planner::PhysicalNode* plan)
      : IExecutor(ctx, exec_type, plan) {
    input_chunk_ = ctx_->arena->CreateObject<chunk::Chunk>(ctx_->arena);
    output_chunk_ = ctx_->arena->CreateObject<chunk::Chunk>(ctx_->arena);
  };

  State Prepare() override;
  Status Work() override;

  InputPort& GetInputPort() { return inputs_.at(0); }
  OutputPort& GetOutputPort() { return outputs_.at(0); }

 protected:
  virtual Status Transform(chunk::Chunk* input_chunk, chunk::Chunk* output_chunk) = 0;

 private:
  chunk::Chunk* input_chunk_;
  chunk::Chunk* output_chunk_;
};
}  // namespace scheduler
}  // namespace amdb