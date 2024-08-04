#include "sql/optimizer/cascades/memo.h"
#include "sql/optimizer/cascades/task.h"

namespace amdb {
namespace opt {

class OptimizeExprTask final : public Task {
 public:
  OptimizeExprTask(const ExprId& expr_id, bool exploring);

  std::vector<std::unique_ptr<Task>> execute(Cascades* optimizer) override;

  std::string describe() override;

 private:
  ExprId expr_id_{0};
  bool exploring_{false};
};
}  // namespace opt
}  // namespace amdb
