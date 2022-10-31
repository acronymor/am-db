#include "sql/storage/table_iterator.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/codec/rc_codec.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace storage {
class TableIteratorTest : public testsuite::SchemaGen {
 protected:
  void SetUp() override {
    testsuite::SchemaGen::SetUp();

    uint64_t db_id = 0;
    uint64_t table_id = 0;
    schema::TableInfo* table_info = GenTableInfo(db_id, table_id);
    table_ = arena_->CreateObject<Table>(instance_, arena_, table_info);
  }

  void TearDown() override { SchemaGen::TearDown(); }

 protected:
  Table* table_;
};

TEST_F(TableIteratorTest, Empty) {
  chunk::RowDescriptor* row_desc = GenRowDesc();
  chunk::Chunk* rows = GenRows(row_desc, 4);
  Status status = table_->Insert(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  TableIterator table_it(table_, 3);

  std::string key1, key2, value;
  codec::EncodeRow(table_->table_info_, rows->select_.at(0), &key1, &value);
  codec::EncodeRow(table_->table_info_, rows->select_.at(2), &key2, &value);

  IteratorOptions it_opt = {
      .lower = key1, .upper = key2, .lower_open = true, .upper_open = false};
  table_it.InitIterOptions(it_opt);

  for (table_it.Open(); table_it.HasNext() && table_it.Code() == Status::C_OK;) {
    chunk::Chunk chunks;
    table_it.Next();
    status = table_it.GetItem(&chunks);
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }
}
};  // namespace storage
};  // namespace amdb