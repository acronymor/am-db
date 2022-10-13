#include "sql/storage/bplus_tree_Iterator.h"

#include "absl/strings/str_format.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "gtest/gtest.h"

namespace amdb {
namespace storage {
class BptIteratorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    StorageAPIOptions options;
    Status status = KvStorageAPISingleton::Init(options);
    AMDB_ASSERT_EQ(Status::C_OK, status);
    instance_ = KvStorageAPISingleton::GetInstance();

    arena_ = new Arena(nullptr);
    tree_ctx_ = new TreeCtx(arena_, instance_);

    BptNonLeafNodeProto root;
    root.set_id(tree_ctx_->AllocateNodeID());
    bptree_ = new Bptree(tree_ctx_, &root);
  }

  void TearDown() override {
    delete instance_, instance_ = nullptr;
    delete tree_ctx_, tree_ctx_ = nullptr;
    delete arena_, arena_ = nullptr;
    delete bptree_, bptree_ = nullptr;
  }

 protected:
  KvStorageAPI* instance_;
  TreeCtx* tree_ctx_;
  Arena* arena_;
  Bptree* bptree_;
};

TEST_F(BptIteratorTest, Iterator) {
  for (size_t i = 0; i < 5; i++) {
    int64_t micros = absl::ToUnixMicros(absl::Now());
    std::string a = absl::StrFormat("a%d-%ld", i, micros);
    bptree_->Insert(std::move(a), std::move(a));
    std::string b = absl::StrFormat("b%d-%ld", i, micros);
    bptree_->Insert(std::move(b), std::move(b));
    std::string c = absl::StrFormat("c%d-%ld", i, micros);
    bptree_->Insert(std::move(c), std::move(c));
    std::string d = absl::StrFormat("d%d-%ld", i, micros);
    bptree_->Insert(std::move(d), std::move(d));
  }

  std::vector<std::string> node_ids;
  std::vector<std::string> kvs;
  tree_ctx_->PullUnsavedTreeNode(&node_ids, &kvs);
  instance_->MPutKV(node_ids, kvs);

  IteratorOptions itoptions = {.lower = "b2",
                               .upper = "c4",
                               .lower_open = true,
                               .upper_open = false};
  BptIterator iterator_(bptree_, &itoptions);
  Status status = iterator_.Open();
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