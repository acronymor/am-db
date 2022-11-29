#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
class PredicatePushdownRule : public RewriteRule {
 public:
  PredicatePushdownRule(StatementContext* ctx) : RewriteRule(ctx, "PredicatePushdownRule") {}

  virtual ~PredicatePushdownRule() = default;

  Status Rewrite(planner::LogicalNode* root, bool* applied) override;

  Status Rewrite(planner::PhysicalNode* root, bool* applied) override;
};
}
}