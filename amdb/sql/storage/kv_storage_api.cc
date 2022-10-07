#include "sql/storage/kv_storage_api.h"
#include "sql/storage/leveldb_client.h"

namespace amdb {
namespace storage {
KvStorageAPI* KvStorageAPISingleton::instance_ = nullptr;

Status KvStorageAPISingleton::Init(const StorageAPIOptions& options) {
  instance_ = LevelDbClient::Create(options);
  return instance_ == nullptr ? Status::C_STORAGE_ERROR
                              : Status::C_OK;
}

KvStorageAPI* KvStorageAPISingleton::GetInstance() { return instance_; }

}  // namespace storage
}  // namespace amdb