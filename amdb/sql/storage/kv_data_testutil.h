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


  }

  void TearDown() override {
    delete instance_, instance_ = nullptr;
    delete arena_, arena_ = nullptr;
  }

 protected:
  KvStorageAPI* instance_;
  Arena* arena_;
};
}  // namespace storage
}  // namespace amdb