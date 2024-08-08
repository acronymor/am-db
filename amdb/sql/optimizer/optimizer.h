#include <cstdint>

namespace amdb {
namespace opt {
enum struct OptimizerType : std::uint32_t { Cascades, Heuristics };

class Optimizer {
 public:
  Optimizer(StatementContext* stmt_ctx) : stmt_ctx(stmt_ctx){};

  virtual ~Optimizer() = default;

  virtual plan::RelOptNode* FindBestExp() = 0;

 public:
  StatementContext* stmt_ctx;
};
}  // namespace opt
}  // namespace amdb