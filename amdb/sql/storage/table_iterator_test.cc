#include "sql/storage/table_iterator.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/storage/kv_data_testutil.h"

namespace amdb {
namespace storage {
class TableIteratorTest : public KvDataTest {
 protected:
  void SetUp() override {
    KvDataTest::SetUp();
  }

 private:
};

TEST_F(TableIteratorTest, CURD) {
  TreeCtx::Schema schema = {.db_id = 0, .table_id = 0, .index_id = 0};
  TreeCtx* tree_ctx = new TreeCtx(instance_, arena_, schema);

  BptNonLeafNodeProto root;
  root.set_id(tree_ctx->AllocateNodeID());
  Bptree* bptree = new Bptree(tree_ctx, &root);
  TableIterator table_it(bptree, tree_ctx, 3);

  IteratorOptions it_opt = { .lower = "0", .upper = "2", .lower_open = true, .upper_open = false};
  table_it.InitIterOptions(it_opt);

  Status status = table_it.Open();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  table_it.Next();

  chunk::Chunk chunks;
  status = table_it.GetItem(&chunks);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}

};  // namespace storage
};  // namespace amdb