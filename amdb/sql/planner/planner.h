#include "common/assert.h"
#include "common/log.h"
#include "sql/common/statuscode.h"
#include "sql/context/statement_context.h"
#include "sql/optimizer/rules/predicate_pushdown.h"

namespace amdb {
namespace planner {

Status BuildPlan(StatementContext* ctx);

class Planner {
 public:
  Planner(StatementContext* ctx) : stmt_ctx_(ctx) {
    opt::RewriteRule* rule = new opt::PredicatePushdownRule(ctx);
    rules_.push_back(rule);
  };

  ~Planner() {
    for(opt::RewriteRule* rule: rules_) {
      delete rule;
    }
  }

  // convert logical plan to physical plan
  Status LogicalToPhysicalPlan();
  // RBO
  Status OptimizeLogicalPlan();
  // CBO
  Status OptimizePhysicalPlan();

  Status AttachFinalPlan();

 private:
  StatementContext* stmt_ctx_;
  std::vector<opt::RewriteRule*> rules_;
};
}  // namespace planner
}  // namespace amdb