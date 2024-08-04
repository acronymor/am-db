#include <optional>

#include "sql/optimizer/cascades/memo.h"
#include "sql/optimizer/cascades/task.h"

namespace amdb {
namespace opt {
struct ContinueTask {
  std::size_t next_group_idx;
  std::vector<plan::Cost> input_cost;
  bool return_from_optimize_group;
};

class OptimizeInputsTask final : public Task {
 public:
  OptimizeInputsTask(const ExprId& expr_id, const std::optional<ContinueTask>& cont, bool pruning);

  std::vector<std::unique_ptr<Task>> execute(Cascades* optimizer) override;

  std::string describe() override;

 private:
  ExprId expr_id_{0};
  std::optional<ContinueTask> continue_from_{std::nullopt};
  bool pruning_{false};
};
}  // namespace opt
}  // namespace amdb
