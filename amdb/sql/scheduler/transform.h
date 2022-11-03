#include "sql/chunk/chunk.h"
#include "sql/scheduler/executor.h"

namespace amdb {
namespace scheduler {
class ITransform : public IExecutor {
 public:
  explicit ITransform(StatementContext* ctx, ExecType exec_type)
      : IExecutor(ctx, exec_type) {
    input_chunk_ =
        ctx_->arena->CreateObject<chunk::Chunk>(ctx_->arena, ctx_->row_desc);
    output_chunk_ =
        ctx_->arena->CreateObject<chunk::Chunk>(ctx_->arena, ctx_->row_desc);
  };

  State Prepare() override;
  Status Work() override;

  InputPort& GetInputPort() { return inputs_[0]; }
  OutputPort& GetOutputPort() { return outputs_[0]; }

 protected:
  virtual Status Transform(chunk::Chunk* input_chunk,
                           chunk::Chunk* output_chunk) = 0;

 private:
  chunk::Chunk* input_chunk_;
  chunk::Chunk* output_chunk_;
};
}  // namespace scheduler
}  // namespace amdb