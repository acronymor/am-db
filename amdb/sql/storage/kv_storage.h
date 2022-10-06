#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "sql/common/statuscode.h"

namespace amdb {
namespace storage {
class KvStorageAPI {
 public:
  Status PutKV(const std::string& key, const std::string& value);
};

}  // namespace storage
}  // namespace amdb
