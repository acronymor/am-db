#include "sql/plan/rel_opt_rule.h"

#include "sql/plan/table_scan.h"

namespace amdb {
namespace plan {
RelOptRule::RelOptRule(StatementContext* stmt_ctx) : stmt_ctx(stmt_ctx) {}

RelOptNode* PhysicalTableScanRule::convert(RelOptNode* node) {
  PhysicalTableScan* physical = stmt_ctx->arena->CreateObject<PhysicalTableScan>();
  LogicalTableScan* logical = dynamic_cast<LogicalTableScan*>(node);
  physical->SetTable(logical->GetTable());
  return physical;
};
}  // namespace plan
}  // namespace amdb