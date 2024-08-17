#include <iostream>
#include <cstdint>
#include "sql/statistics/statistic.h"

namespace amdb {
namespace codec {
size_t EncodeTableStatisticKey(const uint64_t db_id, const uint64_t table_id, std::string* out);
size_t DecodeTableStatisticKey(const std::string& in, stat::StatisticsMeta* meta);
}
} // amdb
