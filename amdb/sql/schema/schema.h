#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "absl/strings/str_format.h"
#include "common/const.h"
#include "sql/schema/schema_type.h"

namespace amdb {
namespace schema {
struct ColumnInfo {
  uint64_t id = amdb::kInvalidColumnID;
  uint64_t table_id = amdb::kInvalidTableID;

  std::string name;
  std::string comment;
};

struct IndexInfo {
  uint64_t id = amdb::kInvalidIndexID;
  uint64_t table_id = amdb::kInvalidTableID;

  std::string name;
  std::string table_name;

  std::vector<ColumnInfo*> columns;

  ConstraintType type;
  std::string comment;
};

struct TableInfo {
  uint64_t id = amdb::kInvalidTableID;
  uint64_t db_id = amdb::kInvalidIDatabaseID;

  std::string name;
  std::string db_name;

  std::unordered_map<std::string, ColumnInfo*> name_to_column;
  std::unordered_map<uint64_t, ColumnInfo*> id_to_column;

  std::vector<ColumnInfo> column_list;
  std::unordered_map<uint64_t, IndexInfo*> id_to_index;
  std::unordered_map<std::string, IndexInfo*> name_to_index;

  std::unordered_map<uint64_t, std::vector<IndexInfo*>> column_id_to_index;
  IndexInfo* primary_index{nullptr};

  std::string FullName() const {
    return absl::StrFormat("%s.%s", db_name, name);
  }

  std::string FullId() const { return absl::StrFormat("%d.%d", db_id, id); }

  uint64_t create_ts;
  uint64_t update_ts;
};

struct DatabaseInfo {
 public:
  uint64_t id = amdb::kInvalidIDatabaseID;
  std::string name;
  std::unordered_set<uint64_t> table_id;
  uint64_t create_ts;
  uint64_t update_ts;
  std::string comment;
};
}  // namespace schema
}  // namespace amdb