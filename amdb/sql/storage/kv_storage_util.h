#include <functional>

#include "sql/common/statuscode.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class KvStorageUtil {
 public:
  explicit KvStorageUtil(KvStorageAPI* storage_api);

  Status GetRecords(
      const std::vector<std::pair<std::string, std::string>>& inputs,
      std::vector<std::pair<std::string, std::string>>* outputs,
      std::function<std::string && (const std::string&)> callback);

  Status GetRecords(const std::vector<std::string>& inputs,
                    std::vector<std::string>* outputs);

 private:
  KvStorageAPI* storage_api_;
};
}  // namespace storage

}  // namespace amdb
