#include "sql/testsuite/schema_gen_testutil.h"

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "sql/storage/bplus_tree_node.h"
#include "sql/storage/index.h"
#include "sql/storage/metadata.h"
#include "sql/testsuite/data_gen_testutil.h"

namespace amdb {
namespace testsuite {
schema::DatabaseInfo* SchemaGen::GenDatabaseInfo(uint64_t db_id) {
  if (auto itr = dbs_.find(toDbKey(db_id)); itr != dbs_.end()) {
    return itr->second;
  }

  schema::DatabaseInfo* database_info = arena_->CreateObject<schema::DatabaseInfo>();
  database_info->id = db_id;
  database_info->name = toName(db_id, kDbName);
  database_info->create_ts = absl::ToUnixMicros(absl::Now());
  database_info->update_ts = absl::ToUnixMicros(absl::Now());

  // ---------------- dbs --------------------
  dbs_.emplace(toDbKey(db_id), database_info);

  return database_info;
}

/*
 * db_name = test-db-1
 * table_name = test-table-1
 * id(long) | name(string) | age(short)
 */
schema::TableInfo* SchemaGen::GenTableInfo(uint64_t db_id, uint64_t table_id) {
  AMDB_ASSERT_TRUE(dbs_.contains(toDbKey(db_id)));

  schema::DatabaseInfo* database_info = GenDatabaseInfo(db_id);

  if (auto itr = tables_.find(toTableKey(db_id, table_id)); itr != tables_.end()) {
    return itr->second;
  }

  schema::TableInfo* table_info_ = arena_->CreateObject<schema::TableInfo>();
  std::string db_name = database_info->name;
  std::string table_name = toName(table_id, kTableName);
  table_info_->column_list.reserve(3);
  table_info_->index_list.reserve(3);

  // ---------------- column list --------------------
  table_info_->column_list.emplace_back();
  schema::ColumnInfo& col_info_01 = table_info_->column_list.at(0);
  col_info_01.id = 0;
  col_info_01.name = "id";
  col_info_01.table_id = table_id;
  col_info_01.table_name = table_name;
  col_info_01.type = {.type = parser::MYSQL_TYPE_LONGLONG, .charset = parser::CharacterSet::CHARSET_UTF8};

  table_info_->column_list.emplace_back();
  schema::ColumnInfo& col_info_02 = table_info_->column_list.at(1);
  col_info_02.id = 1;
  col_info_02.name = "name";
  col_info_02.table_id = table_id;
  col_info_02.table_name = table_name;
  col_info_02.type = {.type = parser::MYSQL_TYPE_STRING, .charset = parser::CharacterSet::CHARSET_UTF8};

  table_info_->column_list.emplace_back();
  schema::ColumnInfo& col_info_03 = table_info_->column_list.at(2);
  col_info_03.id = 2;
  col_info_03.name = "age";
  col_info_03.table_id = table_id;
  col_info_03.table_name = table_name;
  col_info_03.type = {.type = parser::MYSQL_TYPE_SHORT, .charset = parser::CharacterSet::CHARSET_UTF8};

  // ---------------- index list --------------------
  table_info_->index_list.emplace_back();
  schema::IndexInfo& index_info_01 = table_info_->index_list.at(0);
  index_info_01.id = 0;
  index_info_01.table_id = table_id;
  index_info_01.name = "index_id";
  index_info_01.table_name = col_info_01.table_name;
  index_info_01.columns.emplace_back(col_info_01);
  index_info_01.root_node_id = 1;
  index_info_01.max_tree_node_id = 1;
  index_info_01.type = parser::ConstraintType::CONSTRAINT_PRIMARY;

  table_info_->index_list.emplace_back();
  schema::IndexInfo& index_info_02 = table_info_->index_list.at(1);
  index_info_02.id = 1;
  index_info_02.table_id = table_id;
  index_info_02.name = "index_name";
  index_info_02.table_name = col_info_02.table_name;
  index_info_02.root_node_id = 1;
  index_info_02.max_tree_node_id = 1;
  index_info_02.columns.emplace_back(col_info_02);
  index_info_02.type = parser::ConstraintType::CONSTRAINT_INDEX;

  table_info_->index_list.emplace_back();
  schema::IndexInfo& index_info_03 = table_info_->index_list.at(2);
  index_info_03.id = 2;
  index_info_03.table_id = table_id;
  index_info_03.table_name = col_info_03.table_name;
  index_info_03.name = "index_age";
  index_info_03.root_node_id = 1;
  index_info_03.max_tree_node_id = 1;
  index_info_03.columns.emplace_back(col_info_03);
  index_info_03.type = parser::ConstraintType::CONSTRAINT_INDEX;

  // ---------------- table_info --------------------
  table_info_->id = table_id;
  table_info_->name = table_name;
  table_info_->db_id = db_id;
  table_info_->db_name = db_name;
  table_info_->create_ts = absl::ToUnixMicros(absl::Now());
  table_info_->update_ts = absl::ToUnixMicros(absl::Now());

  table_info_->id_to_column.insert({col_info_01.id, &col_info_01});
  table_info_->name_to_column.insert({col_info_01.name, &col_info_01});

  table_info_->primary_index = &index_info_01;
  table_info_->id_to_index.insert({index_info_01.id, &index_info_01});
  table_info_->id_to_index.insert({index_info_02.id, &index_info_02});
  table_info_->id_to_index.insert({index_info_03.id, &index_info_03});
  table_info_->name_to_index.insert({index_info_01.name, &index_info_01});
  table_info_->name_to_index.insert({index_info_02.name, &index_info_02});
  table_info_->name_to_index.insert({index_info_03.name, &index_info_03});
  table_info_->column_id_to_index[col_info_01.id].push_back(&index_info_01);
  table_info_->column_id_to_index[col_info_02.id].push_back(&index_info_02);
  table_info_->column_id_to_index[col_info_03.id].push_back(&index_info_03);

  // ---------------- tables --------------------
  tables_.emplace(toTableKey(db_id, table_id), table_info_);

  return table_info_;
}

Status SchemaGen::DumpMata(uint64_t db_id, uint64_t table_id) {
  if (auto itr = dbs_.find(toDbKey(db_id)); itr == dbs_.end()) {
    return Status::C_DATABASE_NOT_FOUND;
  }

  if (auto itr = tables_.find(toTableKey(db_id, table_id)); itr == tables_.end()) {
    return Status::C_TABLE_NOT_FOUND;
  }

  schema::DatabaseInfo* db_info = dbs_[toDbKey(db_id)];
  schema::TableInfo* table_info = tables_[toTableKey(db_id, table_id)];

  storage::Metadata meta;
  Status status = meta.DumpDatabaseMeta(db_info->id, db_info);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = meta.DumpDatabaseIdByName(db_info->name, &db_info->id);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = meta.DumpTableMeta(table_info->db_id, table_info->id, table_info);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  status = meta.DumpTableIdByName(table_info->db_id, table_info->name, &table_info->id);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  for (schema::IndexInfo& index_info : table_info->index_list) {
    storage::Index* index = arena_->CreateObject<storage::Index>(instance_, arena_, table_info, &index_info);
    status = meta.DumpTreeNode(db_id, table_id, index_info.id, index_info.root_node_id, index->Tree()->Root());
    AMDB_ASSERT_EQ(Status::C_OK, status);
  }

  return Status::C_OK;
}

schema::DatabaseInfo* SchemaGen::GetDatabaseInfo(uint64_t db_id) { return dbs_[toDbKey(db_id)]; }

schema::TableInfo* SchemaGen::GetTableInfo(uint64_t db_id, uint64_t table_id) {
  return tables_[toTableKey(db_id, table_id)];
}
}  // namespace testsuite
}  // namespace amdb