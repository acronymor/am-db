#include "absl/strings/str_format.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "gtest/gtest.h"
#include "sql/storage/bplus_tree.h"
#include "sql/storage/bplus_tree_node.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class KvDataTest : public ::testing::Test {
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
}  // namespace storage
}  // namespace amdb