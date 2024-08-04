#include <sql/optimizer/rules/rule.h>

#include "sql/optimizer/cascades/memo.h"
#include "sql/optimizer/cascades/task.h"

namespace amdb {
namespace opt {

class ApplyRuleTask final : public Task {
 public:
  ApplyRuleTask(const RuleId& rule_id, const ExprId& expr_id, bool exploring);

  std::vector<std::unique_ptr<Task>> execute(Cascades* optimizer) override;

  std::string describe() override;

 private:
  RuleId rule_id_{0};
  ExprId expr_id_{0};
  bool exploring_{false};
};

}  // namespace opt
}  // namespace amdb
