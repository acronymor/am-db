#pragma once

#include "proto/bptree.pb.h"
#include "proto/schema.pb.h"
#include "sql/schema/schema.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class Metadata {
 public:
  Metadata();
  explicit Metadata(KvStorageAPI* kv_api);

  [[nodiscard]] Status LoadDatabaseMeta(uint64_t db_id, schema::DatabaseInfo* database_info);

  [[nodiscard]] Status LoadTableMeta(uint64_t db_id, uint64_t table_id, schema::TableInfo* table_info);

  [[nodiscard]] Status DumpTableMeta(uint64_t db_id, uint64_t table_id, schema::TableInfo* table_info);

  [[nodiscard]] Status LoadIndexMeta(uint64_t db_id, uint64_t table_id, uint64_t index_id, uint64_t node_id,
                                     schema::IndexInfo* index_info);

  [[nodiscard]] Status DumpIndexMeta(uint64_t db_id, uint64_t table_id, schema::IndexInfo* index_info);

  [[nodiscard]] Status LoadTreeNode(uint64_t db_id, uint64_t table_id, uint64_t index_id, uint64_t node_id,
                                    BptNodeProto* root_proto);

 private:
  KvStorageAPI* kv_api_;
};
}  // namespace storage
}  // namespace amdb