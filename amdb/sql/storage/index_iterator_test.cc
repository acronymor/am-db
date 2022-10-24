#include "sql/storage/index_iterator.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/storage/kv_data_testutil.h"

namespace amdb {
namespace storage {
class IndexIteratorTest : public KvDataTest {};

TEST_F(IndexIteratorTest, CRUD) {
  IteratorOptions it_opt = {
      .lower = "b2", .upper = "c4", .lower_open = true, .upper_open = false};

  TreeCtx::Schema schema = {.db_id = 0, .table_id = 0, .index_id = 0};
  TreeCtx* tree_ctx = new TreeCtx(instance_, arena_, schema);

  BptNonLeafNodeProto root;
  root.set_id(tree_ctx->AllocateNodeID());
  Bptree* bptree = new Bptree(tree_ctx, &root);

  IndexIterator index_it(bptree, tree_ctx, 3);
  index_it.InitIterOptions(it_opt);

  Status status = Status::C_OK;

  for (size_t i = 0; i < 5; i++) {
    int64_t micros = absl::ToUnixMicros(absl::Now());
    std::string a = absl::StrFormat("a%d-%ld", i, micros);
    status = bptree->Insert(std::move(a), std::move("value-" + a));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string b = absl::StrFormat("b%d-%ld", i, micros);
    status = bptree->Insert(std::move(b), std::move("value-" + b));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string c = absl::StrFormat("c%d-%ld", i, micros);
    status = bptree->Insert(std::move(c), std::move("value-" + c));
    AMDB_ASSERT_EQ(Status::C_OK, status);
    std::string d = absl::StrFormat("d%d-%ld", i, micros);
    status = bptree->Insert(std::move(d), std::move("value-" + d));
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  std::vector<std::string> node_ids;
  std::vector<std::string> kvs;
  tree_ctx->PullUnsavedTreeNode(&node_ids, &kvs);
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