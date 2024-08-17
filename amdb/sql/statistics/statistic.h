#pragma once

#include <cstdint>
#include <iostream>

#include "common/const.h"
#include "sql/common/statuscode.h"
#include "proto/statistics.pb.h"

namespace amdb {
namespace stat {
struct StatisticsMeta {
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
