#include "sql/planner/planner.h"

#include "sql/planner/plan_node_util.h"
#include "sql/planner/plan_node_visitor.h"

namespace amdb {
namespace planner {
namespace {
Status ToPhyPlan(StatementContext* stmt_ctx, LogicalNode* logical, PhysicalNode* physical) {
  if (logical->children.empty()) {
    return Status::C_OK;
  }

  if (physical == nullptr) {
    physical = ToPhysicalNode(stmt_ctx, logical);
    stmt_ctx->physical_plan = physical;
  }

  for (LogicalNode* node : logical->children) {
    PhysicalNode* p_node = ToPhysicalNode(stmt_ctx, node);
    physical->AddChild(p_node);
    ToPhyPlan(stmt_ctx, node, p_node);
  }

  return Status::C_OK;
}
}  // namespace

Status BuildPlan(StatementContext* ctx) {
  Planner plan(ctx);

  RETURN_ERR_NOT_OK(plan.OptimizeLogicalPlan());
  RETURN_ERR_NOT_OK(plan.LogicalToPhysicalPlan());
  RETURN_ERR_NOT_OK(plan.OptimizePhysicalPlan());
  RETURN_ERR_NOT_OK(plan.AttachFinalPlan());

  return Status::C_OK;
}

Status Planner::LogicalToPhysicalPlan() {
  ToPhyPlan(stmt_ctx_, stmt_ctx_->logical_plan, stmt_ctx_->physical_plan);
  return Status::C_OK;
}

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