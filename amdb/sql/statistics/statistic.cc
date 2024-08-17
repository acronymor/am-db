#include "sql/statistics/statistic.h"

namespace amdb {
namespace stat {
Status StatisticsMeta::Serialize(StatisticsMetaProto* output) {
  output->set_db_id(this->db_id);
  output->set_table_id(this->table_id);
  output->set_is_analyzing(this->is_analyzing);
  output->set_create_ts(this->create_ts);
  output->set_update_ts(this->update_ts);
  return Status::C_OK;
};

Status StatisticsMeta::Deserialize(const StatisticsMetaProto& input) {
  this->db_id = input.db_id();
  this->table_id = input.table_id();
  this->is_analyzing = input.is_analyzing();
  this->create_ts = input.create_ts();
  this->update_ts = input.update_ts();
  return Status::C_OK;
};
}  // namespace stat
}  // namespace amdb