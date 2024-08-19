#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "common/const.h"
#include "proto/statistics.pb.h"
#include "sql/common/statuscode.h"
#include "sql/statistics/histogram.h"

namespace amdb {
namespace stat {
struct ColumnStatistics {
  std::uint64_t db_id = amdb::kInvalidIDatabaseID;
  std::uint64_t table_id = amdb::kInvalidTableID;
  std::uint64_t column_id = amdb::kInvalidColumnID;
  std::string min;
  std::string max;
  std::uint64_t count;
  std::uint64_t size;
  std::uint64_t ndv;
  std::uint64_t create_ts;
  std::uint64_t update_ts;
  Histogram histogram;

  // serialize to proto
  Status Serialize(ColumnStatisticsProto* output);
  // deserialize from proto
  Status Deserialize(const ColumnStatisticsProto& input);
};

struct MetaStatistics {
  uint64_t db_id = amdb::kInvalidIDatabaseID;
  uint64_t table_id = amdb::kInvalidTableID;
  bool is_analyzing = false;
  uint64_t create_ts;
  uint64_t update_ts;
  std::string comment;

  // serialize to proto
  Status Serialize(StatisticsMetaProto* output);
  // deserialize from proto
  Status Deserialize(const StatisticsMetaProto& input);
};
};  // namespace stat
}  // namespace amdb
