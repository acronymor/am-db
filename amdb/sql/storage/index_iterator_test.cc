#include "sql/storage/index_iterator.h"

#include "absl/strings/str_format.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace storage {
class IndexIteratorTest : public testsuite::SchemaGen {
 protected:
  void SetUp() override {
    testsuite::SchemaGen::SetUp();

    uint64_t db_id = 0;
    uint64_t table_id = 0;
    schema::TableInfo* table_info = GenTableInfo(db_id, table_id);

    BptNonLeafNodeProto* root = arena_->CreateObject<BptNonLeafNodeProto>();
    root->set_id(0);
    index_ = new Index(instance_, arena_, table_info, table_info->primary_index,
                       root);
  }

  void TearDown() { delete index_, index_ = nullptr; }

 protected:
  Index* index_;
};

TEST_F(IndexIteratorTest, CRUD) {
  IteratorOptions it_opt = {
      .lower = "b2", .upper = "c4", .lower_open = true, .upper_open = false};

  IndexIterator index_it(index_, 3);
  index_it.InitIterOptions(it_opt);

  Status status = Status::C_OK;

  for (size_t i = 0; i < 5; i++) {
    int64_t micros = absl::ToUnixMicros(absl::Now());
    std::string a = absl::StrFormat("a%d-%ld", i, micros);
    status = index_->Tree()->Insert(std::move(a), std::move("value-" + a));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string b = absl::StrFormat("b%d-%ld", i, micros);
    status = index_->Tree()->Insert(std::move(b), std::move("value-" + b));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string c = absl::StrFormat("c%d-%ld", i, micros);
    status = index_->Tree()->Insert(std::move(c), std::move("value-" + c));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string d = absl::StrFormat("d%d-%ld", i, micros);
    status = index_->Tree()->Insert(std::move(d), std::move("value-" + d));
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  std::vector<std::string> node_ids;
  std::vector<std::string> kvs;
  index_->TreeCtxx()->PullUnsavedTreeNode(&node_ids, &kvs);
  status = instance_->MPutKV(node_ids, kvs);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = index_it.Open();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  index_it.Next();

  chunk::Chunk chunks;
  status = index_it.GetItem(&chunks);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}

};  // namespace storage
};  // namespace amdb