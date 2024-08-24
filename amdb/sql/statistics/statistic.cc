#include "sql/statistics/statistic.h"

#include <common/assert.h>

#include "sql/codec/codec.h"

namespace amdb {
namespace stat {
Status ColumnStatistics::Serialize(ColumnStatisticsProto* output) {
  output->set_db_id(this->db_id);
  output->set_table_id(this->table_id);
  output->set_column_id(this->column_id);
  output->set_min(this->min);
  output->set_max(this->max);
  output->set_count(this->count);
  output->set_size(this->size);
  output->set_ndv(this->ndv);
  output->set_create_ts(this->create_ts);
  output->set_update_ts(this->update_ts);

  auto to_string = [](const expr::ExprValue& value) -> std::string {
    std::string s;
    codec::EncodeExprValue(value, &s);
    return s;
  };

  for (const auto& bucket : this->histogram.GetBuckets()) {
    const auto tmp = output->add_histogram();
    tmp->set_start(std::move(to_string(bucket.start)));
    tmp->set_end(std::move(to_string(bucket.end)));
    tmp->set_count(bucket.count);
    tmp->set_ndv(bucket.ndv);
    tmp->set_cf(bucket.cf);
  }

  return Status::C_OK;
}

Status ColumnStatistics::Deserialize(const ColumnStatisticsProto& input) {
  this->db_id = input.db_id();
  this->table_id = input.table_id();
  this->column_id = input.column_id();
  this->min = input.min();
  this->max = input.max();
  this->count = input.count();
  this->size = input.size();
  this->ndv = input.ndv();
  this->create_ts = input.create_ts();
  this->update_ts = input.update_ts();
  this->histogram = Histogram(static_cast<std::size_t>(input.histogram().size()));

  for (const auto& bucket : input.histogram()) {
    Histogram::Bucket tmp;
    tmp.Deserialize(bucket);
    Status status = this->histogram.AddBucket(tmp);
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  return Status::C_OK;
}

Status MetaStatistics::Serialize(StatisticsMetaProto* output) {
  output->set_db_id(this->db_id);
  output->set_table_id(this->table_id);
  output->set_is_analyzing(this->is_analyzing);
  output->set_create_ts(this->create_ts);
  output->set_update_ts(this->update_ts);
  return Status::C_OK;
};

Status MetaStatistics::Deserialize(const StatisticsMetaProto& input) {
  this->db_id = input.db_id();
  this->table_id = input.table_id();
  this->is_analyzing = input.is_analyzing();
  this->create_ts = input.create_ts();
  this->update_ts = input.update_ts();
  return Status::C_OK;
};
}  // namespace stat
}  // namespace amdb