#include "sql/storage/bplus_tree.h"

#include <gtest/gtest.h>

#include "common/assert.h"

namespace amdb {
namespace storage {
class BplusTreeTest : public testing::Test {
 protected:
  void SetUp() override {
    StorageAPIOptions options;
    Status status = KvStorageAPISingleton::Init(options);
    AMDB_ASSERT_EQ(Status::C_OK, status);
    instance_ = KvStorageAPISingleton::GetInstance();

    BptNonLeafNodeProto root;
    bptree_ = new Bptree(&root);
    tree_ctx_ = new TreeCtx(instance_);
  }

  void TearDown() override {
    delete instance_, instance_ = nullptr;
    delete bptree_, bptree_ = nullptr;
    delete tree_ctx_, tree_ctx_ = nullptr;
  }

 protected:
  KvStorageAPI* instance_;
  TreeCtx* tree_ctx_;
  Bptree* bptree_;
};

TEST_F(BplusTreeTest, CRUD) {
  Status status = bptree_->Insert(tree_ctx_, "key-a", "value-a");
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = bptree_->Insert(tree_ctx_, "key-b", "value-b");
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::string value;
  status = bptree_->GetItem(tree_ctx_, "key-a", &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_EQ("value-a", value);

  status = bptree_->Delete(tree_ctx_, "key-b");
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = bptree_->GetItem(tree_ctx_, "key-a", &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace storage
}  // namespace amdb
