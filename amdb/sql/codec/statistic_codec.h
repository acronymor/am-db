#include <cstdint>
#include <string>

namespace amdb {
namespace codec {
size_t EncodeMetaStatistic(const std::uint64_t db_id, const std::uint64_t table_id, std::string* out);

size_t EncodeColumnStatistic(const std::uint64_t db_id, const std::uint64_t table_id, const std::uint64_t column_id,
                             std::string* out);
}  // namespace codec
}  // namespace amdb