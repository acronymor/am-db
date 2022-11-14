#include "sql/storage/metadata.h"

#include "bplus_tree_node.h"
#include "common/assert.h"
#include "sql/codec/schema_codec.h"

namespace amdb {
namespace storage {
Metadata::Metadata(storage::KvStorageAPI *kv_api) : kv_api_(kv_api) {}

Status Metadata::LoadDatabaseMeta(uint64_t db_id,
                                  schema::DatabaseInfo *database_info) {
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

Status Metadata::DumpTableMeta(uint64_t db_id, uint64_t table_id,
                               schema::TableInfo *table_info) {
  std::string key;
  codec::EncodeTableID(db_id, table_id, &key);

  schema::TableProto proto;
  Status status = table_info->Serialize(&proto);
  RETURN_ERR_NOT_OK(status);

  std::string value = proto.SerializeAsString();
  status = kv_api_->PutKV(key, value);
  return status;
}

Status Metadata::LoadTableMeta(uint64_t db_id, uint64_t table_id,
                               schema::TableInfo *table_info) {
  std::string key;
  codec::EncodeTableID(db_id, table_id, &key);

  std::string value;
  Status status = kv_api_->GetKV(key, &value);
  RETURN_ERR_NOT_OK(status);

  schema::TableProto proto;

  if (!proto.ParseFromString(value)) {
    return Status::C_TABLE_SCHEMA_ERROR;
  }

  return table_info->Deserialize(proto);
}

Status Metadata::LoadIndexMeta(uint64_t db_id, uint64_t table_id,
                               uint64_t index_id, uint64_t node_id,
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

Status Metadata::DumpIndexMeta(uint64_t db_id, uint64_t table_id,
                               schema::IndexInfo *index_info) {
  std::string key;
  codec::EncodeIndexTreeNodeID(db_id, table_id, index_info->id,
                               index_info->root_node_id, &key);

  schema::IndexProto proto;
  Status status = index_info->Serialize(&proto);
  RETURN_ERR_NOT_OK(status);

  status = kv_api_->PutKV(key, proto.SerializeAsString());
  return status;
}

Status Metadata::LoadTreeNode(uint64_t db_id, uint64_t table_id,
                              uint64_t index_id, uint64_t node_id,
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

}  // namespace storage
}  // namespace amdb