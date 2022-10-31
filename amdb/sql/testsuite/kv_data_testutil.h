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

  const std::unique_ptr<leveldb::DB>& RawKvClient() {
    return dynamic_cast<storage::LevelDbClient*>(instance_)->GetDb();
  }

  void ClearAll() {
    leveldb::Iterator* it = RawKvClient()->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
      leveldb::Status status = RawKvClient()->Delete(leveldb::WriteOptions(), it->key());
      assert(it->status().ok());  // Check for any errors found during the scan
    }
    delete it;
  }

  Arena* GetArena() { return arena_; }

  storage::KvStorageAPI* GetStorage() { return instance_; }

 protected:
  storage::KvStorageAPI* instance_;
  Arena* arena_;
};
}  // namespace testsuite
}  // namespace amdb