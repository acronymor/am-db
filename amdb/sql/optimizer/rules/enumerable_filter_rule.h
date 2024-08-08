#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class EnumerableFilterRule final : public Rule {
 public:
  explicit EnumerableFilterRule();

  std::shared_ptr<RuleMatcher> Matcher() override;

  plan::RelOptNode* Apply(const Optimizer* optimizer, plan::RelOptNode* node) override;
};

};  // namespace opt
}  // namespace amdb
