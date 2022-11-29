#include "sql/planner/planner.h"

namespace amdb {
namespace planner {

Status BuildPlan(StatementContext* ctx) {
  Planner plan(ctx);

  RETURN_ERR_NOT_OK(plan.AstToLogicalPlan());
  RETURN_ERR_NOT_OK(plan.OptimizeLogicalPlan());
  RETURN_ERR_NOT_OK(plan.LogicalToPhysicalPlan());
  RETURN_ERR_NOT_OK(plan.OptimizePhysicalPlan());
  RETURN_ERR_NOT_OK(plan.AttachFinalPlan());

  return Status::C_OK;
}

Status Planner::AstToLogicalPlan() { return Status::C_OK; }

Status Planner::LogicalToPhysicalPlan() { return Status::C_OK; }

Status Planner::OptimizeLogicalPlan() {
  for (opt::RewriteRule* rule : rules_) {
    bool applied = false;
    Status status = rule->Rewrite(stmt_ctx_->logical_plan, &applied);
    DEBUG("logical optimize with {}, and status is {}", rule->name, status);
  }
  return Status::C_OK;
}

Status Planner::OptimizePhysicalPlan() {
  for (opt::RewriteRule* rule : rules_) {
    bool applied = false;
    Status status = rule->Rewrite(stmt_ctx_->physical_plan, &applied);
    DEBUG("physical optimize with {}, and status is {}", rule->name, status);
  }
  return Status::C_OK;
}

Status Planner::AttachFinalPlan() {
  stmt_ctx_->resolved_plan = stmt_ctx_->physical_plan;
  return Status::C_OK;
}
}  // namespace planner
}  // namespace amdb