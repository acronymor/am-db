#include "gtest/gtest.h"
#include "common/assert.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace testsuite {
class KvDataTest : public ::testing::Test {
 protected:
  void SetUp() override {
    storage::StorageAPIOptions options;
    Status status = storage::KvStorageAPISingleton::Init(options);
    AMDB_ASSERT_EQ(Status::C_OK, status);
    instance_ = storage::KvStorageAPISingleton::GetInstance();
    arena_ = new Arena(nullptr);
  }

  Arena* GetArena() { return arena_; }

  storage::KvStorageAPI* GetStorage() { return instance_; }

  void TearDown() override {
    delete instance_, instance_ = nullptr;
    delete arena_, arena_ = nullptr;
  }

 protected:
  storage::KvStorageAPI* instance_;
  Arena* arena_;
};
}  // namespace testsuite
}  // namespace amdb