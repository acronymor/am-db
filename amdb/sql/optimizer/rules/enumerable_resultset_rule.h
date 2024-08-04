#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class EnumerableResultSetRule final: public Rule {
 public:
  explicit EnumerableResultSetRule();

  std::shared_ptr<RuleMatcher> Matcher() override;

  std::vector<plan::RelOptNode*> Apply(const Optimizer* optimizer,
                                       const std::unordered_map<std::size_t, plan::RelOptNode*>& input) override;
};

};  // namespace opt
}  // namespace amdb
