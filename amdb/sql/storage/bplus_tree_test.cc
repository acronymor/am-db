#include "sql/storage/bplus_tree.h"

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "common/assert.h"
#include "gtest/gtest.h"

namespace amdb {
namespace storage {
class BplusTreeTest : public testing::Test {
 protected:
  void SetUp() override {
    StorageAPIOptions options;
    Status status = KvStorageAPISingleton::Init(options);
    AMDB_ASSERT_EQ(Status::C_OK, status);
    instance_ = KvStorageAPISingleton::GetInstance();

    arena_ = new Arena(nullptr);

    schema_ = {.db_id = 0, .table_id = 0, .index_id = 0};
    tree_ctx_ = new TreeCtx(instance_, arena_, schema_);
  }

  void TearDown() override {
    delete instance_, instance_ = nullptr;
    delete tree_ctx_, tree_ctx_ = nullptr;
    delete arena_, arena_ = nullptr;
  }

 protected:
  KvStorageAPI* instance_;
  TreeCtx* tree_ctx_;
  Arena* arena_;
  TreeCtx::Schema schema_;
};

TEST_F(BplusTreeTest, CRUD) {
  BptNodeProto proto;
  proto.set_id(tree_ctx_->AllocateNodeID());

  BptNode* root = arena_->CreateObject<BptNode>(tree_ctx_, &proto);
  Bptree bptree(tree_ctx_, root);

  std::string key1 = "key-a" + std::to_string(absl::ToUnixMicros(absl::Now()));
  std::string key2 =
      "key-b" + std::to_string(absl::ToUnixMicros(absl::Now()) + 1000);

  Status status = bptree.Insert(std::move(key1), "value-a");
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = bptree.Insert(std::move(key2), "value-b");
  AMDB_ASSERT_EQ(Status::C_OK, status);

  tree_ctx_->PullUnsavedTreeNode();
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::string value;
  status = bptree.GetItem(key1, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_EQ("value-a", value);

  status = bptree.Delete(key2);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = bptree.GetItem(key2, &value);
  AMDB_ASSERT_EQ(Status::C_STORAGE_KV_NOT_FOUND, status);

  status = bptree.GetItem(key1, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
}
}  // namespace storage
}  // namespace amdb
