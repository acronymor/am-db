#include "sql/optimizer/planner.h"

#include "common/log.h"
#include "sql/optimizer/cascades/cascades.h"

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
      node = CreateDatabase(stmt_ctx, logical_create_database);
    }; break;

    case plan::RelOptNodeType::kLogicalCreateTable: {
      plan::LogicalCreateTable* logical_create_table = dynamic_cast<plan::LogicalCreateTable*>(logical);
      node = CreateTable(stmt_ctx, logical_create_table);
    }; break;

    case plan::RelOptNodeType::kLogicalInsert: {
      plan::LogicalInsert* logical_insert = dynamic_cast<plan::LogicalInsert*>(logical);
      node = Insert(stmt_ctx, logical_insert);
    }; break;

    default:
      // ERROR("Not Support LogicalNode type {}", logical->GetType());
      break;
  }
  return node;
}

plan::PhysicalCreateDatabase* CreateDatabase(const StatementContext* stmt_ctx, plan::LogicalCreateDatabase* logical) {
  plan::PhysicalCreateDatabase* node = stmt_ctx->arena->CreateObject<plan::PhysicalCreateDatabase>();
  node->SetTable(logical->GetTable());
  return node;
}

static plan::PhysicalCreateTable* CreateTable(const StatementContext* stmt_ctx, plan::LogicalCreateTable* logical) {
  plan::PhysicalCreateTable* node = stmt_ctx->arena->CreateObject<plan::PhysicalCreateTable>();
  node->SetTable(logical->GetTable());
  return node;
}

static plan::PhysicalInsert* Insert(const StatementContext* stmt_ctx, plan::LogicalInsert* logical) {
  plan::PhysicalInsert* node = stmt_ctx->arena->CreateObject<plan::PhysicalInsert>();
  node->SetTable(logical->GetTable());
  node->ExprNodes().assign(logical->ExprNodes().begin(), logical->ExprNodes().end());
  return node;
}
}  // namespace opt
}  // namespace amdb
