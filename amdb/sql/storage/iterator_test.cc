#include "sql/storage/iterator.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/storage/kv_data_testutil.h"

namespace amdb {
namespace storage {
class IteratorTest : public KvDataTest {};

TEST_F(IteratorTest, IndexIteratorTest) {
  IteratorOptions it_opt = {
      .lower = "b2", .upper = "c4", .lower_open = true, .upper_open = false};

  IndexIterator index_it(bptree_, tree_ctx_, 3);
  index_it.InitIterOptions(it_opt);

  for (size_t i = 0; i < 5; i++) {
    int64_t micros = absl::ToUnixMicros(absl::Now());
    std::string a = absl::StrFormat("a%d-%ld", i, micros);
    bptree_->Insert(std::move(a), std::move("value-" + a));
    std::string b = absl::StrFormat("b%d-%ld", i, micros);
    bptree_->Insert(std::move(b), std::move("value-" + b));
    std::string c = absl::StrFormat("c%d-%ld", i, micros);
    bptree_->Insert(std::move(c), std::move("value-" + c));
    std::string d = absl::StrFormat("d%d-%ld", i, micros);
    bptree_->Insert(std::move(d), std::move("value-" + d));
  }

  std::vector<std::string> node_ids;
  std::vector<std::string> kvs;
  tree_ctx_->PullUnsavedTreeNode(&node_ids, &kvs);
  instance_->MPutKV(node_ids, kvs);

  Status status = index_it.Open();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  index_it.Next();

  chunk::Chunk chunks;
  status = index_it.GetItem(&chunks);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}

};  // namespace storage
};  // namespace amdb