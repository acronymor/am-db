#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/context/statement_context.h"
#include "sql/testsuite/table_gen_testutil.h"

namespace amdb {
namespace testsuite {
class TableDataGen : public testing::Test {
 protected:
  void SetUp() override {
    uint64_t db_id = 0;
    uint64_t table_id = 0;

    schema::DatabaseInfo* db_info = table_gen.GenDatabaseInfo(db_id);
    AMDB_ASSERT_TRUE(db_info != nullptr);
    schema::TableInfo* table_info = table_gen.GenTableInfo(db_id, table_id);
    AMDB_ASSERT_TRUE(table_info != nullptr);
    table_ = table_gen.GetArena()->CreateObject<storage::Table>(table_gen.GetArena(), table_info);

    ctx = new StatementContext();
    ctx->arena = table_gen.GetArena();

    // --------------------

    Status status = table_gen.DumpMata(table_->table_info_->db_id, table_->table_info_->id);
    AMDB_ASSERT_EQ(Status::C_OK, status);

    chunk::Chunk* rows = table_gen.GenRows(table_->table_info_, 10);
    status = table_->Insert(rows);
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  void TearDown() override {
    delete ctx, ctx = nullptr;
    table_gen.ClearAll();
  }

 protected:
  StatementContext* ctx;
  storage::Table* table_;
  testsuite::TableGen table_gen;
};
};  // namespace testsuite
}  // namespace amdb