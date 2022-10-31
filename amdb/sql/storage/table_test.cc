#include "sql/storage/table.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/codec/kv_codec.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace storage {
class TableTest : public testsuite::SchemaGen {
 protected:
  void SetUp() override {
    SchemaGen::SetUp();

    uint64_t db_id = 0;
    uint64_t table_id = 0;
    schema::TableInfo* table_info = GenTableInfo(db_id, table_id);
    table_ = arena_->CreateObject<Table>(instance_, arena_, table_info);
  }

  void TearDown() override {
    SchemaGen::TearDown();
  }

 protected:
  Table* table_;
};

TEST_F(TableTest, CURD) {
  chunk::Chunk* rows = GenRows(GenRowDesc(), 2);
  Status status = table_->Insert(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = table_->Delete(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);
};
}  // namespace storage
}  // namespace amdb