#include "sql/storage/kv_storage.h"

namespace amdb {
namespace storage {

Status KvStorageAPI::PutKV(const std::string& key, const std::string& value) {
  return Status::C_OK;
}
}  // namespace storage
}  // namespace amdb