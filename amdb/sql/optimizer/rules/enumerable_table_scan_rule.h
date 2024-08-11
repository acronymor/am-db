#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class EnumerableTableScanRule final : public Rule {
 public:
  explicit EnumerableTableScanRule(Arena* arena);

  RuleMatcher* Matcher() override;

  plan::RelOptNode* Apply(plan::RelOptNode* node) override;
};

};  // namespace opt
};  // namespace amdb
