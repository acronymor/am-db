#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/chunk/chunk_util.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace storage {
class TableTest : public testsuite::TableDataGen {};

TEST_F(TableTest, CURD) {
  chunk::Chunk* rows = table_gen.GenRows(table_->table_info_, 10);
  Status status = table_->Insert(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = table_->Delete(rows);
  AMDB_ASSERT_EQ(Status::C_OK, status);
};
}  // namespace storage
}  // namespace amdb