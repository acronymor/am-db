#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class EnumerableFilterRule final : public Rule {
 public:
  explicit EnumerableFilterRule(Arena* arena);

  RuleMatcher* Matcher() override;

  plan::RelOptNode* Apply(plan::RelOptNode* node) override;
};

};  // namespace opt
}  // namespace amdb
