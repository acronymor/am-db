#include "sql/storage/bplus_tree_iterator.h"

#include "absl/strings/str_format.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "gtest/gtest.h"
#include "sql/testsuite/kv_data_testutil.h"

namespace amdb {
namespace storage {
class BptIteratorTest : public testsuite::KvDataTest {
 protected:
  void SetUp() override {
    KvDataTest::SetUp();

    TreeCtx::Schema schema = {.db_id = 0, .table_id = 0, .index_id = 0};
    tree_ctx_ = arena_->CreateObject<TreeCtx>(instance_, arena_, schema);

    BptNonLeafNodeProto root;
    root.set_id(tree_ctx_->AllocateNodeID());
    bptree_ = arena_->CreateObject<Bptree>(tree_ctx_, &root);
  }

 protected:
  Bptree* bptree_;
  TreeCtx* tree_ctx_;
};

TEST_F(BptIteratorTest, Iterator) {
  Status status = Status::C_OK;
  for (size_t i = 0; i < 5; i++) {
    int64_t micros = absl::ToUnixMicros(absl::Now());
    std::string a = absl::StrFormat("a%d-%ld", i, micros);
    status = bptree_->Insert(std::move(a), std::move("value-" + a));
    AMDB_ASSERT_EQ(Status::C_OK, status);

    std::string b = absl::StrFormat("b%d-%ld", i, micros);
    status = bptree_->Insert(std::move(b), std::move("value-" + b));
    AMDB_ASSERT_EQ(Status::C_OK, status);

    std::string c = absl::StrFormat("c%d-%ld", i, micros);
    status = bptree_->Insert(std::move(c), std::move("value-" + c));
    AMDB_ASSERT_EQ(Status::C_OK, status);

    std::string d = absl::StrFormat("d%d-%ld", i, micros);
    status = bptree_->Insert(std::move(d), std::move("value-" + d));
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  std::vector<std::string> node_ids;
  std::vector<std::string> kvs;
  tree_ctx_->PullUnsavedTreeNode(&node_ids, &kvs);
  status = instance_->MPutKV(node_ids, kvs);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  IteratorOptions itoptions = {
      .lower = "b2", .upper = "c4", .lower_open = true, .upper_open = false};
  BptIterator iterator_(bptree_, &itoptions);
  status = iterator_.Open();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  while (iterator_.HashNext()) {
    std::vector<std::pair<std::string, std::string>> vectors;
    size_t row_count = 3;
    status = iterator_.Next(&vectors, row_count);
    AMDB_ASSERT_EQ(Status::C_OK, status);
    AMDB_ASSERT_GE(row_count, vectors.size());
    AMDB_ASSERT_TRUE(IsCmpGe(DataCmp(vectors.begin()->first, "b2")));
    AMDB_ASSERT_TRUE(IsCmpLe(DataCmp(vectors.back().first, "c4")));
  }
}
}  // namespace storage
}  // namespace amdb