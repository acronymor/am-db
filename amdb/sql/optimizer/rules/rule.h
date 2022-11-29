#include "sql/common/statuscode.h"
#include "sql/planner/logical_plan_node.h"
#include "sql/planner/physical_plan_node.h"

namespace amdb {
namespace opt {
class RewriteRule {
 public:
  RewriteRule(StatementContext* ctx, const std::string&& name) : name(std::move(name)) {}
  virtual ~RewriteRule() = default;

  virtual Status Rewrite(planner::LogicalNode* root, bool* applied) = 0;

  virtual Status Rewrite(planner::PhysicalNode* root, bool* applied) = 0;

  std::string name;
  StatementContext* stmt_ctx;
};
}  // namespace opt
}  // namespace amdb