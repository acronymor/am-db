#include "sql/storage/id_allocator.h"

#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace storage {
class IndexIteratorTest : public testsuite::TableDataGen {
 protected:
  void SetUp() override { id_allocator_ = new IdAllocator(4); }

  void TearDown() { delete id_allocator_, id_allocator_ = nullptr; }

 protected:
  IdAllocator* id_allocator_;
};

TEST_F(IndexIteratorTest, BatchAllocateID) {
  std::string key = "a";
  std::vector<uint64_t> ids;
  Status status = id_allocator_->BatchAllocateID(IDType::Database, key, 2, &ids);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_EQ(2, ids.size());
}
}  // namespace storage
}  // namespace amdb