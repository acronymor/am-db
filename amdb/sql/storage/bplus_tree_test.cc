#include "sql/storage/bplus_tree.h"

#include <gtest/gtest.h>

#include "absl/time/clock.h"
#include "absl/time/time.h"
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

    tree_ctx_ = new TreeCtx(instance_);
  }

  void TearDown() override {
    delete instance_, instance_ = nullptr;
    delete tree_ctx_, tree_ctx_ = nullptr;
  }

 protected:
  KvStorageAPI* instance_;
  TreeCtx* tree_ctx_;
};

TEST_F(BplusTreeTest, CRUD) {
  BptNonLeafNodeProto root;
  root.set_id(tree_ctx_->AllocateNodeID());
  Bptree bptree(&root);

  std::string key1 = "key-a" + std::to_string(absl::ToUnixMicros(absl::Now()));
  std::string key2 = "key-b" + std::to_string(absl::ToUnixMicros(absl::Now()) + 1000);

  Status status = bptree.Insert(tree_ctx_, std::move(key1), "value-a");
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = bptree.Insert(tree_ctx_, std::move(key2), "value-b");
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::vector<std::string> node_ids;
  std::vector<std::string> kvs;
  tree_ctx_->PullUnsavedTreeNode(&node_ids, &kvs);
  instance_->MPutKV(node_ids, kvs);

  std::string value;
  status = bptree.GetItem(tree_ctx_, key1, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_EQ("value-a", value);

  status = bptree.Delete(tree_ctx_, key2);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = bptree.GetItem(tree_ctx_, key2, &value);
  AMDB_ASSERT_EQ(Status::C_STORAGE_KV_NOT_FOUND, status);

  status = bptree.GetItem(tree_ctx_, key1, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace storage
}  // namespace amdb
