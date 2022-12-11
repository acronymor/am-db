#include "sql/storage/metadata.h"

#include "bplus_tree_node.h"
#include "common/assert.h"
#include "sql/codec/schema_codec.h"

namespace amdb {
namespace storage {

Metadata::Metadata() { kv_api_ = KvStorageAPISingleton::GetInstance(); }

Metadata::Metadata(storage::KvStorageAPI *kv_api) : kv_api_(kv_api) {}

Status Metadata::DumpDatabaseMeta(uint64_t db_id, schema::DatabaseInfo *database_info) {
  std::string key;
  codec::EncodeDatabaseID(db_id, &key);

  schema::DatabaseProto proto;
  Status status = database_info->Serialize(&proto);
  RETURN_ERR_NOT_OK(status);

  std::string value = proto.SerializeAsString();
  status = kv_api_->PutKV(key, value);
  return status;
}

Status Metadata::LoadDatabaseMeta(uint64_t db_id, schema::DatabaseInfo *database_info) {
  std::string key;
  codec::EncodeDatabaseID(db_id, &key);

  std::string value;
  Status status = kv_api_->GetKV(key, &value);
  RETURN_ERR_NOT_OK(status);

  schema::DatabaseProto proto;
  if (!proto.ParseFromString(value)) {
    return Status::C_DATABASE_SCHEMA_ERROR;
  }

  return database_info->Deserialize(proto);
}

Status Metadata::ModifyDatabaseMeta(uint64_t db_id, std::function<Status(schema::DatabaseInfo *)> callback) {
  schema::DatabaseInfo database_info;
  Status status = LoadDatabaseMeta(db_id, &database_info);
  RETURN_ERR_NOT_OK(status);

  status = callback(&database_info);
  RETURN_ERR_NOT_OK(status);

  std::string key;
  codec::EncodeDatabaseID(db_id, &key);
  status = kv_api_->DelKV(key);
  RETURN_ERR_NOT_OK(status);

  status = DumpDatabaseMeta(db_id, &database_info);
  RETURN_ERR_NOT_OK(status);

  return Status::C_OK;
}

Status Metadata::DumpTableMeta(uint64_t db_id, uint64_t table_id, schema::TableInfo *table_info) {
  std::string key;
  codec::EncodeTableID(db_id, table_id, &key);

  schema::TableProto proto;
  Status status = table_info->Serialize(&proto);
  RETURN_ERR_NOT_OK(status);

  std::string value = proto.SerializeAsString();
  status = kv_api_->PutKV(key, value);
  return status;
}

Status Metadata::LoadTableMeta(uint64_t db_id, uint64_t table_id, schema::TableInfo *table_info) {
  std::string key;
  codec::EncodeTableID(db_id, table_id, &key);

  std::string value;
  Status status = kv_api_->GetKV(key, &value);
  if (Status::C_STORAGE_KV_NOT_FOUND == status) {
    return Status::C_TABLE_NOT_FOUND;
  }

  schema::TableProto proto;

  if (!proto.ParseFromString(value)) {
    return Status::C_TABLE_SCHEMA_ERROR;
  }

  return table_info->Deserialize(proto);
}

Status Metadata::LoadIndexMeta(uint64_t db_id, uint64_t table_id, uint64_t index_id, uint64_t node_id,
                               schema::IndexInfo *index_info) {
  std::string key;
  codec::EncodeIndexTreeNodeID(db_id, table_id, index_id, node_id, &key);

  std::string value;
  Status status = kv_api_->GetKV(key, &value);
  RETURN_ERR_NOT_OK(status);

  schema::IndexProto proto;

  if (!proto.ParseFromString(value)) {
    return Status::C_BPTREE_ROOT_NODE_ERROR;
  }

  return index_info->Deserialize(proto);
}

Status Metadata::DumpIndexMeta(uint64_t db_id, uint64_t table_id, schema::IndexInfo *index_info) {
  std::string key;
  codec::EncodeIndexTreeNodeID(db_id, table_id, index_info->id, index_info->root_node_id, &key);

  schema::IndexProto proto;
  Status status = index_info->Serialize(&proto);
  RETURN_ERR_NOT_OK(status);

  status = kv_api_->PutKV(key, proto.SerializeAsString());
  return status;
}

Status Metadata::LoadTreeNode(uint64_t db_id, uint64_t table_id, uint64_t index_id, uint64_t node_id,
                              BptNodeProto *root_proto) {
  std::string key;
  codec::EncodeIndexTreeNodeID(db_id, table_id, index_id, node_id, &key);

  std::string value;
  Status status = kv_api_->GetKV(key, &value);
  RETURN_ERR_NOT_OK(status);

  if (root_proto->ParseFromString(value)) {
    return Status::C_BPTREE_ROOT_NODE_ERROR;
  }
  return Status::C_OK;
}

Status Metadata::LoadDatabaseIdByName(const std::string &db_name, uint64_t *db_id) {
  std::string key;
  codec::EncodeDatabaseName(db_name, &key);

  std::string value;
  Status status = kv_api_->GetKV(key, &value);
  if (Status::C_STORAGE_KV_NOT_FOUND == status) {
    return Status::C_DATABASE_NOT_FOUND;
  }

  codec::DecodeDatabaseID(value, db_id);
  return status;
}

Status Metadata::DumpDatabaseIdByName(const std::string &db_name, uint64_t *db_id) {
  std::string key;
  codec::EncodeDatabaseName(db_name, &key);

  std::string value;
  codec::EncodeDatabaseID(*db_id, &value);

  return kv_api_->PutKV(key, value);
}

Status Metadata::LoadTableIdByName(uint64_t db_id, const std::string &table_name, uint64_t *table_id) {
  std::string key;
  codec::EncodeTableName(db_id, table_name, &key);

  std::string value;
  Status status = kv_api_->GetKV(key, &value);
  if (Status::C_STORAGE_KV_NOT_FOUND == status) {
    return Status::C_TABLE_NOT_FOUND;
  }

  uint64_t db_id_store;
  codec::DecodeTableID(value, &db_id_store, table_id);
  AMDB_ASSERT_EQ(db_id, db_id_store);
  return Status::C_OK;
}

Status Metadata::DumpTableIdByName(uint64_t db_id, const std::string &table_name, uint64_t *table_id) {
  std::string key;
  codec::EncodeTableName(db_id, table_name, &key);

  std::string value;
  codec::EncodeTableID(db_id, *table_id, &value);

  return kv_api_->PutKV(key, value);
}

}  // namespace storage
}  // namespace amdb