#include <cstdint>

namespace amdb {
namespace opt {
enum struct OptimizerType : std::uint32_t { Cascades, Heuristics };

class Optimizer {
 public:
  virtual ~Optimizer() = default;

  virtual plan::RelOptNode* FindBestExp() = 0;
};
}  // namespace opt
}  // namespace amdb