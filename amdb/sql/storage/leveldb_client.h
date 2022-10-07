#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <memory>

#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class LevelDbClient : public KvStorageAPI {
 public:
  ~LevelDbClient() override = default;

  Status PutKV(const std::string& key, const std::string& value) override;

  Status MPutKV(const std::vector<std::string>& keys,
                const std::vector<std::string>& values) override;

  Status MPutKV(
      const std::unordered_map<std::string, std::string>& kv_map) override;

  Status GetKV(const std::string& key, std::string* value) override;

  Status MGetKV(const std::vector<std::string>& keys,
                std::vector<std::string>* values) override;

  Status DelKV(const std::string& key) override;

  Status MDelKV(const std::vector<std::string>& keys) override;

  Status Incrby(const std::string& key, int64_t step,
                int64_t* new_val) override;

  static LevelDbClient* Create(const StorageAPIOptions& options);

 private:
  explicit LevelDbClient(std::unique_ptr<leveldb::DB>&& db)
      : db_(std::move(db)) {};
  std::unique_ptr<leveldb::DB> db_;
};
}  // namespace storage
}  // namespace amdb