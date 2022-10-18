#include "sql/storage/table.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/storage/kv_data_testutil.h"

namespace amdb {
namespace storage {
class TableTest : public KvDataTest {};

TEST_F(TableTest, CURD){};
}  // namespace storage
}  // namespace amdb