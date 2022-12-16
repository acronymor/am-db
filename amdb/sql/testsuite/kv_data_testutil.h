#include "common/assert.h"
#include "sql/storage/leveldb_client.h"

namespace amdb {
namespace testsuite {
class KvDataTest {
 public:
  KvDataTest() {
    storage::StorageAPIOptions options;
    Status status = storage::KvStorageAPISingleton::Init(options);
    AMDB_ASSERT_EQ(Status::C_OK, status);
    instance_ = storage::KvStorageAPISingleton::GetInstance();
    arena_ = new Arena(nullptr);
  }

  ~KvDataTest() {
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

  storage::KvStorageAPI* GetKvApi() { return instance_; }

 protected:
  storage::KvStorageAPI* instance_;
  Arena* arena_;
};
}  // namespace testsuite
}  // namespace amdb