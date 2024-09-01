#include "sql/statistics/statistics.h"

#include <common/assert.h>
#include <sql/codec/statistic_codec.h>

namespace amdb {
namespace stat {
Statistics::Statistics() { kv_api_ = storage::KvStorageAPISingleton::GetInstance(); }

Statistics::Statistics(storage::KvStorageAPI* kv_api) : kv_api_(kv_api) {}

Status Statistics::LoadMetaStatistic(std::uint64_t db_id, std::uint64_t table_id, MetaStatistic* statistic) {
  std::string key;
  codec::EncodeMetaStatistic(db_id, table_id, &key);
  std::string value;
  Status status = this->kv_api_->GetKV(key, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  StatisticsMetaProto proto;
  proto.ParseFromString(value);

  status = statistic->Deserialize(proto);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  return status;
}

Status Statistics::DumpMetaStatistic(std::uint64_t db_id, std::uint64_t table_id, MetaStatistic* statistic) {
  std::string key;
  codec::EncodeMetaStatistic(db_id, table_id, &key);

  StatisticsMetaProto proto;
  Status status = statistic->Serialize(&proto);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = this->kv_api_->PutKV(key, proto.SerializeAsString());
  AMDB_ASSERT_EQ(Status::C_OK, status);

  return status;
}

Status Statistics::LoadColumnStatistic(std::uint64_t db_id, std::uint64_t table_id, std::uint64_t colum_id,
                                       ColumnStatistic* statistic) {
  std::string key;
  codec::EncodeColumnStatistic(db_id, table_id, colum_id, &key);
  std::string value;
  Status status = this->kv_api_->GetKV(key, &value);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  ColumnStatisticsProto proto;
  proto.ParseFromString(value);

  status = statistic->Deserialize(proto);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  return status;
}

Status Statistics::DumpColumnStatistic(std::uint64_t db_id, std::uint64_t table_id, std::uint64_t colum_id,
                                       ColumnStatistic* statistic) {
  std::string key;
  codec::EncodeColumnStatistic(db_id, table_id, colum_id, &key);

  ColumnStatisticsProto proto;
  Status status = statistic->Serialize(&proto);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  status = this->kv_api_->PutKV(key, proto.SerializeAsString());
  AMDB_ASSERT_EQ(Status::C_OK, status);

  return status;
}
}  // namespace stat
}  // namespace amdb
