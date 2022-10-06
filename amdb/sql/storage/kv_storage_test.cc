#include "gtest/gtest.h"
#include "sql/storage/kv_storage.h"

namespace amdb {
namespace storage {
class StorageTest : public testing::Test {};

TEST_F(StorageTest, Put) {
  KvStorageAPI api;
  Status status = api.PutKV("key", "value");
  ASSERT_EQ(Status::C_OK, status);
}
}  // namespace storage
}  // namespace amdb
