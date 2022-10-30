#include "sql/storage/leveldb_client.h"

#include <vector>

#include "common/assert.h"
#include "gtest/gtest.h"

namespace amdb {
namespace storage {
namespace {
void __printf__(const std::string& key, const std::string& value) {
  printf("key=");
  for (size_t i = 0; i < key.length(); i++) {
    printf("0x%02hhX ", key.at(i));
  }
  printf(", value=");
  for (size_t i = 0; i < value.length(); i++) {
    printf("0x%02hhX ", value.at(i));
  }
  printf("\n");
}
}  // namespace

class LevelDbClientTest : public testing::Test {
 protected:
  void SetUp() override {
    StorageAPIOptions options;
    client_ = LevelDbClient::Create(options);
  }

 protected:
  LevelDbClient* client_;
};

TEST_F(LevelDbClientTest, CRUD) {
  std::string key = "key";
  std::string value = "hello world";

  Status status = client_->PutKV(key, value);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::string tmp_value;
  status = client_->GetKV(key, &tmp_value);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = client_->DelKV(key);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = client_->GetKV(key, &tmp_value);
  AMDB_ASSERT_EQ(Status::C_STORAGE_KV_NOT_FOUND, status);
}

TEST_F(LevelDbClientTest, BATCH_CRUD) {
  std::string key1 = "key1";
  std::string value1 = "value1";

  std::string key2 = "key2";
  std::string value2 = "value2";

  std::vector<std::string> keys = {key1, key2};
  std::vector<std::string> values = {value1, value2};

  Status status = client_->MPutKV(keys, values);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::vector<std::string> tmp_values;
  tmp_values.reserve(values.size());
  status = client_->MGetKV(keys, &tmp_values);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::vector<std::string> tmp_keys;
  tmp_keys.push_back(key2);
  status = client_->MDelKV(tmp_keys);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  std::string tmp_value;
  status = client_->GetKV(key1, &tmp_value);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = client_->GetKV(key2, &tmp_value);
  AMDB_ASSERT_EQ(Status::C_STORAGE_KV_NOT_FOUND, status);
}

TEST_F(LevelDbClientTest, INC) {
  std::string key = "inc_key";
  int64_t value;

  Status status = client_->DelKV(key);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  int64_t step = 2;
  status = client_->Incrby(key, 2, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_EQ(2, value);

  step = 1022;
  status = client_->Incrby(key, step, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_EQ(1024, value);
}

TEST_F(LevelDbClientTest, Scan) {
  leveldb::Iterator* it = client_->GetDb()->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    __printf__(it->key().ToString(), it->value().ToString());
  }
}
}  // namespace storage
}  // namespace amdb