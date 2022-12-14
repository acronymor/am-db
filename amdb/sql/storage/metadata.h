#pragma once

#include "proto/bptree.pb.h"
#include "proto/schema.pb.h"
#include "sql/schema/schema.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
class BptNode;

class Metadata {
 public:
  Metadata();
  explicit Metadata(KvStorageAPI* kv_api);

  [[nodiscard]] Status LoadDatabaseMeta(uint64_t db_id, schema::DatabaseInfo* database_info);

  [[nodiscard]] Status DumpDatabaseMeta(uint64_t db_id, schema::DatabaseInfo* database_info);

  [[nodiscard]] Status ModifyDatabaseMeta(uint64_t db_id, std::function<Status(schema::DatabaseInfo*)> callback);

  [[nodiscard]] Status LoadDatabaseIdByName(const std::string& db_name, uint64_t* db_id);

  [[nodiscard]] Status DumpDatabaseIdByName(const std::string& db_name, uint64_t* db_id);

  [[nodiscard]] Status LoadTableMeta(uint64_t db_id, uint64_t table_id, schema::TableInfo* table_info);

  [[nodiscard]] Status DumpTableMeta(uint64_t db_id, uint64_t table_id, schema::TableInfo* table_info);

  [[nodiscard]] Status LoadTableIdByName(uint64_t db_id, const std::string& table_name, uint64_t* table_id);

  [[nodiscard]] Status DumpTableIdByName(uint64_t db_id, const std::string& table_name, uint64_t* table_id);

  [[nodiscard]] Status LoadTreeNode(uint64_t db_id, uint64_t table_id, uint64_t index_id, uint64_t node_id, BptNode* node);

  [[nodiscard]] Status DumpTreeNode(uint64_t db_id, uint64_t table_id, uint64_t index_id, uint64_t node_id, BptNode* node);

 private:
  KvStorageAPI* kv_api_;
};
}  // namespace storage
}  // namespace amdb