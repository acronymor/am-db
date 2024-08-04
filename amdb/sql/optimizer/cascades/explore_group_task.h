#include "sql/optimizer/cascades/cascades.h"
#include "sql/optimizer/cascades/memo.h"
#include "sql/optimizer/cascades/task.h"

namespace amdb {
namespace opt {
class ExploreGroupTask final : public Task {
 public:
  ExploreGroupTask(const GroupId& group_id);

  std::vector<std::unique_ptr<Task>> execute(Cascades* optimizer) override;

  std::string describe() override;

 private:
  GroupId group_id_{0};
};
}  // namespace opt
}  // namespace amdb
