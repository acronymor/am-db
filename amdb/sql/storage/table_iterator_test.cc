#include "sql/storage/table_iterator.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/codec/rc_codec.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace storage {
class TableIteratorTest : public testsuite::TableDataGen {};

TEST_F(TableIteratorTest, Empty) {
  std::string key1, key2, value;
  chunk::Chunk* rows = table_gen.GenRows(table_->table_info_, 10);
  codec::EncodeRow(table_->table_info_, rows->select_.at(0), &key1, &value);
  codec::EncodeRow(table_->table_info_, rows->select_.at(2), &key2, &value);

  plan::IndexRange ir;
  ir.lower.type = plan::IndexRange::RangePointType::LEFT_CLOSE;
  ir.lower_str = key1;
  ir.upper.type = plan::IndexRange::RangePointType::RIGHT_OPEN;
  ir.upper_str = key2;

  TableIterator table_it(table_, 3);
  table_it.AddRange(&ir);

  for (table_it.Open(); table_it.HasNext() && table_it.Code() == Status::C_OK;) {
    chunk::Chunk chunks;
    table_it.Next();
    AMDB_ASSERT_EQ(Status::C_OK, table_it.GetItem(&chunks));
  }
}
};  // namespace storage
};  // namespace amdb