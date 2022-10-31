#pragma once

#include "proto/bptree.pb.h"
#include "proto/schema.pb.h"
#include "sql/schema/schema.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class Metadata {
 public:
  explicit Metadata(KvStorageAPI* kv_api);

  Status LoadDatabaseMeta(uint64_t db_id, schema::DatabaseInfo* database_info);

  Status LoadTableMeta(uint64_t db_id, uint64_t table_id,
                      schema::TableInfo* table_info);

  Status DumpTableMeta(uint64_t db_id, uint64_t  table_id,schema::TableInfo* table_info);


  Status LoadIndexMeta(uint64_t db_id, uint64_t table_id, uint64_t index_id,
                          uint64_t node_id, schema::IndexInfo* index_info);

  Status DumpIndexMeta(uint64_t db_id, uint64_t table_id, schema::IndexInfo* index_info);

  Status LoadTreeNode(uint64_t db_id, uint64_t table_id, uint64_t index_id, uint64_t node_id, BptNodeProto* root_proto);

 private:
  KvStorageAPI* kv_api_;
};
}  // namespace storage
}  // namespace amdb