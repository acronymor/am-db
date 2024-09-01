#include <sql/storage/kv_storage_api.h>

#include <cstdint>
#include <iostream>

#include "sql/statistics/statistic.h"

namespace amdb {
namespace stat {
class Statistics {
 public:
  Statistics();
  explicit Statistics(storage::KvStorageAPI* kv_api);

  [[nodiscard]] Status LoadMetaStatistic(std::uint64_t db_id, std::uint64_t table_id,
                                          MetaStatistic* statistic);

  [[nodiscard]] Status DumpMetaStatistic(std::uint64_t db_id, std::uint64_t table_id,
                                          MetaStatistic* statistic);

  [[nodiscard]] Status LoadColumnStatistic(std::uint64_t db_id, std::uint64_t table_id, std::uint64_t colum_id,
                                            ColumnStatistic* statistic);

  [[nodiscard]] Status DumpColumnStatistic(std::uint64_t db_id, std::uint64_t table_id, std::uint64_t colum_id,
                                            ColumnStatistic* statistic);

 private:
  storage::KvStorageAPI* kv_api_;
};
}  // namespace stat
}  // namespace amdb
