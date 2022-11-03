#include "sql/scheduler/transform.h"

namespace amdb {
namespace executor {
class FilterExecutor : public scheduler::ITransform {
 public:
  FilterExecutor(StatementContext* ctx, ExecType exec_type) : ITransform(ctx, exec_type){};

  Status Open() override;
  Status Close() override;

 protected:
  Status Transform(chunk::Chunk *input_chunk,
                   chunk::Chunk *output_chunk) override;
};
}  // namespace executor
}  // namespace amdb