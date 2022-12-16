#include "sql/storage/index_iterator.h"

#include "absl/strings/str_format.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "common/assert.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace storage {
class IndexIteratorTest : public testsuite::TableDataGen {};

TEST_F(IndexIteratorTest, CRUD) {
  planner::IndexRange ir;
  ir.lower.type = planner::IndexRange::RangePointType::LEFT_CLOSE;
  ir.lower_str = "b2";
  ir.upper.type = planner::IndexRange::RangePointType::RIGHT_OPEN;
  ir.upper_str = "c4";

  Index* index = table_->row_index;
  IndexIterator indexit(index, 3);
  indexit.AddRange(&ir);

  Status status = Status::C_OK;

  for (size_t i = 0; i < 5; i++) {
    int64_t micros = absl::ToUnixMicros(absl::Now());
    std::string a = absl::StrFormat("a%d-%ld", i, micros);
    status = index->Tree()->Insert(std::move(a), std::move("value-" + a));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string b = absl::StrFormat("b%d-%ld", i, micros);
    status = index->Tree()->Insert(std::move(b), std::move("value-" + b));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string c = absl::StrFormat("c%d-%ld", i, micros);
    status = index->Tree()->Insert(std::move(c), std::move("value-" + c));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string d = absl::StrFormat("d%d-%ld", i, micros);
    status = index->Tree()->Insert(std::move(d), std::move("value-" + d));
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  index->TreeCtxx()->PullUnsavedTreeNode();
  status = index->Save();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = indexit.Open();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  indexit.Next();

  chunk::Chunk chunks;
  status = indexit.GetItem(&chunks);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}

};  // namespace storage
};  // namespace amdb