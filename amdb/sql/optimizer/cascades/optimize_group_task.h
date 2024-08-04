#include "sql/optimizer/cascades/memo.h"
#include "sql/optimizer/cascades/task.h"

namespace amdb {
namespace opt {

class OptimizeGroupTask final : public Task {
 public:
  OptimizeGroupTask(const GroupId& group_id);

  std::vector<std::unique_ptr<Task>> execute(Cascades* optimizer) override;

  std::string describe() override;

 private:
  GroupId group_id_{0};
};
}  // namespace opt
}  // namespace amdb
