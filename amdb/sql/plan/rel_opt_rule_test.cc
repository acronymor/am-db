#include "rel_opt_rule.h"

#include "sql/plan/rel_opt_table.h"
#include "sql/testsuite/table_testutil.h"
#include "table_scan.h"

namespace amdb {
namespace plan {
struct RelOptRuleTest : public testsuite::TableDataGen {};

TEST_F(RelOptRuleTest, LogicalTableScanToPhyscialTableScan) {
  uint64_t db_id = table_->table_info_->db_id;
  uint64_t table_id = table_->table_info_->id;

  schema::DatabaseInfo* db_info = table_gen.GetDatabaseInfo(db_id);
  schema::TableInfo* table_info = table_gen.GetTableInfo(db_id, table_id);

  RelOptTable* table = ctx->arena->CreateObject<RelOptTable>();
  table->Init(db_info, table_info);

  LogicalTableScan* logical_table_scan = ctx->arena->CreateObject<LogicalTableScan>();

  PhysicalTableScanRule* rule = ctx->arena->CreateObject<PhysicalTableScanRule>(ctx);
  PhysicalTableScan* physical_table_scan = dynamic_cast<PhysicalTableScan*>(rule->convert(logical_table_scan));
  AMDB_ASSERT_TRUE(logical_table_scan->GetTable() == physical_table_scan->GetTable());
}

}  // namespace plan
}  // namespace amdb
