#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class EnumerableResultSetRule final: public Rule {
 public:
  explicit EnumerableResultSetRule(Arena* arena);

  RuleMatcher* Matcher() override;

  plan::RelOptNode* Apply(plan::RelOptNode* node) override;
};

};  // namespace opt
}  // namespace amdb
