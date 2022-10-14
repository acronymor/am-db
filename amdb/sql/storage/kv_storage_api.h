#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "sql/common/statuscode.h"

namespace amdb {
namespace storage {
class KvStorageAPI {
 public:
  virtual ~KvStorageAPI() = default;
  [[nodiscard]] virtual Status PutKV(const std::string& key,
                                     const std::string& value) = 0;

  [[nodiscard]] virtual Status MPutKV(
      const std::vector<std::string>& keys,
      const std::vector<std::string>& values) = 0;

  [[nodiscard]] virtual Status MPutKV(
      const std::unordered_map<std::string, std::string>& kv_map) = 0;

  [[nodiscard]] virtual Status GetKV(const std::string& key,
                                     std::string* value) = 0;

  [[nodiscard]] virtual Status MGetKV(const std::vector<std::string>& keys,
                                      std::vector<std::string>* values) = 0;

  [[nodiscard]] virtual Status DelKV(const std::string& key) = 0;

  [[nodiscard]] virtual Status MDelKV(const std::vector<std::string>& keys) = 0;

  [[nodiscard]] virtual Status Incrby(const std::string& key, int64_t step,
                                      int64_t* new_val) = 0;
};

struct StorageAPIOptions {};

class KvStorageAPISingleton {
 public:
  [[nodiscard]] static Status Init(const StorageAPIOptions& options);
  static KvStorageAPI* GetInstance();

#ifdef AMDB_BUILD_TEST
 public:
#else
 private:
#endif
  static KvStorageAPI* instance_;
};
}  // namespace storage
}  // namespace amdb
