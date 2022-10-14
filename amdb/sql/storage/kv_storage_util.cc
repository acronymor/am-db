#include "sql/storage/kv_storage_util.h"

#include "common/assert.h"

namespace amdb {
namespace storage {

KvStorageUtil::KvStorageUtil(KvStorageAPI *storage_api)
    : storage_api_(storage_api) {}

Status KvStorageUtil::GetRecords(
    const std::vector<std::pair<std::string, std::string>> &inputs,
    std::vector<std::pair<std::string, std::string>> *outputs,
    std::function<std::string && (const std::string &)> callback) {
  std::vector<std::string> keys;
  for (const auto &input : inputs) {
    keys.emplace_back(callback(input.first));
    outputs->emplace_back(input.first, "");
  }

  std::vector<std::string> data;
  Status status = GetRecords(keys, &data);
  RETURN_ERR_NOT_OK(status);

  for (size_t i = 0; i < data.size(); i++) {
    outputs->at(i).second = data[i];
  }

  return Status::C_OK;
}

Status KvStorageUtil::GetRecords(const std::vector<std::string> &inputs,
                                 std::vector<std::string> *outputs) {
  return storage_api_->MGetKV(inputs, outputs);
}

}  // namespace storage
}  // namespace amdb