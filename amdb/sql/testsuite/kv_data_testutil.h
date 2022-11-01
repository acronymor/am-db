#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/storage/leveldb_client.h"

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

  void TearDown() override {
    delete instance_, instance_ = nullptr;
    delete arena_, arena_ = nullptr;
  }

  void ClearAll() {
    std::vector<std::string> keys;
    std::vector<std::string> values;
    Status status = instance_->Scan(keys, values);
    AMDB_ASSERT_EQ(Status::C_OK, status);

    status = instance_->MDelKV(keys);
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  Arena* GetArena() { return arena_; }

  storage::KvStorageAPI* GetStorage() { return instance_; }

 protected:
  storage::KvStorageAPI* instance_;
  Arena* arena_;
};
}  // namespace testsuite
}  // namespace amdb