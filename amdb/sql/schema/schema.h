#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "absl/strings/str_format.h"
#include "common/const.h"
#include "proto/schema.pb.h"
#include "sql/common/statuscode.h"
#include "sql/schema/schema_type.h"

namespace amdb {
namespace schema {
struct ColumnType {
  MysqlType type;
  CharacterSet charset = CHARSET_NULL;
};

struct ColumnInfo {
  uint64_t id = amdb::kInvalidColumnID;
  uint64_t table_id = amdb::kInvalidTableID;

  std::string name;
  std::string table_name;

  ColumnType type;

  std::string comment;

  // serialize to proto
  Status Serialize(ColumnProto* output);
  // deserialize from proto
  Status Deserialize(const ColumnProto& input);
};

struct IndexInfo {
  uint64_t id = amdb::kInvalidIndexID;
  uint64_t table_id = amdb::kInvalidTableID;

  std::string name;
  std::string table_name;

  std::vector<ColumnInfo> columns;

  uint64_t root_node_id;

  ConstraintType type;
  std::string comment;

  // serialize to proto
  Status Serialize(IndexProto* output);
  // deserialize from proto
  Status Deserialize(const IndexProto& input);
};

struct TableInfo {
  uint64_t id = amdb::kInvalidTableID;
  uint64_t db_id = amdb::kInvalidIDatabaseID;

  std::string name;
  std::string db_name;

  // all column
  std::vector<ColumnInfo> column_list;
  std::unordered_map<std::string, ColumnInfo*> name_to_column;
  std::unordered_map<uint64_t, ColumnInfo*> id_to_column;

  // all index which contain primary_index
  std::vector<IndexInfo> index_list;
  std::unordered_map<uint64_t, IndexInfo*> id_to_index;
  std::unordered_map<std::string, IndexInfo*> name_to_index;

  std::unordered_map<uint64_t, std::vector<IndexInfo*>> column_id_to_index;
  IndexInfo* primary_index{nullptr};

  uint64_t create_ts;
  uint64_t update_ts;

  // serialize to proto
  Status Serialize(TableProto* output);
  // deserialize from proto
  Status Deserialize(const TableProto& input);

  std::string FullName() const {
    return absl::StrFormat("%s.%s", db_name, name);
  }

  std::string FullId() const { return absl::StrFormat("%d.%d", db_id, id); }
};

struct DatabaseInfo {
 public:
  uint64_t id = amdb::kInvalidIDatabaseID;
  std::string name;
  std::unordered_set<uint64_t> table_ids;
  uint64_t create_ts;
  uint64_t update_ts;
  std::string comment;

  // serialize to proto
  Status Serialize(DatabaseProto* output);
  // deserialize from proto
  Status Deserialize(const DatabaseProto& input);
};
}  // namespace schema
}  // namespace amdb