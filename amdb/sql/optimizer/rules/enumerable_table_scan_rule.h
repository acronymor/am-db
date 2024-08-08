#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class EnumerableTableScanRule final : public Rule {
 public:
  explicit EnumerableTableScanRule();

  std::shared_ptr<RuleMatcher> Matcher() override;

  plan::RelOptNode* Apply(const Optimizer* optimizer, plan::RelOptNode* node) override;
};

};  // namespace opt
};  // namespace amdb
