#include "sql/storage/leveldb_client.h"

#include <string>

#include "common/assert.h"
#include "common/log.h"
#include "gflags/gflags.h"

namespace amdb {
DECLARE_string(leveldb_dir);

namespace storage {

Status LevelDbClient::PutKV(const std::string& key, const std::string& value) {
  leveldb::Status status = db_->Put(leveldb::WriteOptions(), key, value);
  if (!status.ok()) {
    ERROR("{}", status.ToString());
    return Status::C_STORAGE_ERROR;
  }
  return Status::C_OK;
}

Status LevelDbClient::MPutKV(const std::vector<std::string>& keys,
                             const std::vector<std::string>& values) {
  AMDB_ASSERT_EQ(keys.size(), values.size());
  leveldb::WriteBatch batch;
  for (size_t i = 0; i < keys.size() && i < values.size(); i++) {
    batch.Put(keys.at(i), values.at(i));
  }
  leveldb::Status status = db_->Write(leveldb::WriteOptions(), &batch);
  if (!status.ok()) {
    ERROR("{}", status.ToString());
    return Status::C_STORAGE_ERROR;
  }
  return C_OK;
}

Status LevelDbClient::MPutKV(
    const std::unordered_map<std::string, std::string>& kv_map) {
  leveldb::WriteBatch batch;
  for (auto& entry : kv_map) {
    batch.Put(entry.first, entry.second);
  }
  leveldb::Status status = db_->Write(leveldb::WriteOptions(), &batch);
  if (!status.ok()) {
    ERROR("{}", status.ToString());
    return Status::C_STORAGE_ERROR;
  }
  return C_OK;
}

Status LevelDbClient::GetKV(const std::string& key, std::string* value) {
  leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, value);
  if (status.IsNotFound()) {
    DEBUG("{}", status.ToString());
    return Status::C_STORAGE_KV_NOT_FOUND;
  }

  if (!status.ok()) {
    ERROR("{}", status.ToString());
    return Status::C_STORAGE_ERROR;
  }
  return C_OK;
}

Status LevelDbClient::MGetKV(const std::vector<std::string>& keys,
                             std::vector<std::string>* values) {
  for (auto& key : keys) {
    std::string value;
    Status status = GetKV(key, &value);
    if (status != Status::C_OK) {
      return Status::C_STORAGE_ERROR;
    }
    values->emplace_back(value);
  }

  AMDB_ASSERT_EQ(keys.size(), values->size());

  return C_OK;
}

Status LevelDbClient::DelKV(const std::string& key) {
  leveldb::Status status = db_->Delete(leveldb::WriteOptions(), key);
  if (!status.ok()) {
    ERROR("{}", status.ToString());
    return Status::C_STORAGE_ERROR;
  }
  return Status::C_OK;
}

Status LevelDbClient::MDelKV(const std::vector<std::string>& keys) {
  leveldb::WriteBatch batch;
  for (size_t i = 0; i < keys.size(); i++) {
    batch.Delete(keys.at(i));
  }
  leveldb::Status status = db_->Write(leveldb::WriteOptions(), &batch);
  if (!status.ok()) {
    ERROR("{}", status.ToString());
    return Status::C_STORAGE_ERROR;
  }
  return C_OK;
}

Status LevelDbClient::Incrby(const std::string& key, int64_t step,
                             int64_t* new_val) {
  std::string value = "0";
  leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &value);
  if (status.IsNotFound()) {
    status = db_->Put(leveldb::WriteOptions(), key, value);
  }

  if (status.ok()) {
    leveldb::WriteBatch batch;
    *new_val = std::stoll(value) + step;
    batch.Delete(key);
    batch.Put(key, std::to_string(*new_val));
    status = db_->Write(leveldb::WriteOptions(), &batch);
  }

  if (!status.ok()) {
    ERROR("{}", status.ToString());
    return Status::C_STORAGE_ERROR;
  }

  return C_OK;
}

#ifdef AMDB_BUILD_TEST
Status LevelDbClient::Scan(std::vector<std::string>& keys, std::vector<std::string>& values) {
  leveldb::Iterator* it = db_->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    keys.push_back(it->key().ToString());
    values.push_back(it->value().ToString());
  }
  AMDB_ASSERT_EQ(keys.size(), values.size());
  delete it;
  return C_OK;
}
#endif

LevelDbClient* LevelDbClient::Create(const StorageAPIOptions& options) {
  leveldb::DB* db;
  leveldb::Options option;
  option.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(option, FLAGS_leveldb_dir, &db);
  if (!status.ok()) {
    ERROR("{}", status.ToString());
  }
  AMDB_ASSERT_TRUE(status.ok());
  std::unique_ptr<leveldb::DB> db1(db);
  return new LevelDbClient(std::move(db1));
}
}  // namespace storage
}  // namespace amdb