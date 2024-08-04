#include "sql/optimizer/planner.h"
#include "sql/optimizer/cascades/cascades.h"

#include "common/log.h"

namespace amdb {
namespace opt {
Status BuildPlan(StatementContext* ctx) {
  if (ctx->stmt_ast->node_type != parser::NodeType::NT_SELECT) {
    ctx->physical_plan = opt::ToPhysicalNode(ctx, ctx->logical_plan);
  } else {
    ctx->physical_plan = Cascades(ctx).FindBestExp();
  }
  return Status::C_OK;
}

plan::RelOptNode* ToPhysicalNode(StatementContext* stmt_ctx, plan::RelOptNode* logical) {
  plan::RelOptNode* node = nullptr;
  switch (logical->GetType()) {
    case plan::RelOptNodeType::kLogicalCreateDatabase: {
      plan::LogicalCreateDatabase* logical_create_database = dynamic_cast<plan::LogicalCreateDatabase*>(logical);
      node = dynamic_cast<plan::PhysicalCreateDatabase*>(CreateDatabase(stmt_ctx, logical_create_database));
    }; break;

    case plan::RelOptNodeType::kLogicalCreateTable: {
      plan::LogicalCreateTable* logical_create_table = dynamic_cast<plan::LogicalCreateTable*>(logical);
      node = dynamic_cast<plan::PhysicalCreateTable*>(CreateTable(stmt_ctx, logical_create_table));
    }; break;

    case plan::RelOptNodeType::kLogicalInsert: {
      plan::LogicalInsert* logical_insert = dynamic_cast<plan::LogicalInsert*>(logical);
      node = dynamic_cast<plan::PhysicalInsert*>(Insert(stmt_ctx, logical_insert));
    }; break;

    default:
      // ERROR("Not Support LogicalNode type {}", logical->GetType());
      break;
  }
  return node;
}

plan::PhysicalCreateDatabase* CreateDatabase(StatementContext* stmt_ctx, plan::LogicalCreateDatabase* logical) {
  plan::PhysicalCreateDatabase* node = stmt_ctx->arena->CreateObject<plan::PhysicalCreateDatabase>();
  node->SetTable(logical->GetTable());
  return node;
}

static plan::PhysicalCreateTable* CreateTable(StatementContext* stmt_ctx, plan::LogicalCreateTable* logical) {
  plan::PhysicalCreateTable* node = stmt_ctx->arena->CreateObject<plan::PhysicalCreateTable>();
  node->SetTable(logical->GetTable());
  return node;
}

static plan::PhysicalInsert* Insert(StatementContext* stmt_ctx, plan::LogicalInsert* logical) {
  plan::PhysicalInsert* node = stmt_ctx->arena->CreateObject<plan::PhysicalInsert>();
  node->SetTable(logical->GetTable());
  return node;
}
}  // namespace opt
}  // namespace amdb
