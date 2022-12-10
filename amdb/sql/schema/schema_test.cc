#include "sql/schema/schema.h"

#include "common/assert.h"
#include "gtest/gtest.h"

namespace amdb {
namespace schema {
class SchemaTest : public ::testing::Test {
 protected:
  void SetUp() override {}
};

TEST_F(SchemaTest, Schema) {
  ColumnInfo col_info0;
  col_info0.id = 0;
  col_info0.table_id = 0;
  col_info0.name = "id";
  col_info0.table_name = "test_table";
  col_info0.comment = "";

  ColumnInfo col_info1;
  col_info1.id = 1;
  col_info1.table_id = 0;
  col_info1.name = "name";
  col_info1.table_name = "test_table";
  col_info1.comment = "";

  ColumnProto col_proto;
  Status status = col_info1.Serialize(&col_proto);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  AMDB_ASSERT_EQ(col_info1.id, col_proto.id());
  AMDB_ASSERT_EQ(col_info1.table_name, col_proto.table_name());

  ColumnInfo col_info_tmp;
  col_info_tmp.Deserialize(col_proto);
  AMDB_ASSERT_EQ(col_info1.id, col_info_tmp.id);
  AMDB_ASSERT_EQ(col_info1.table_name, col_info_tmp.table_name);

  //-------------------------------------------
  IndexInfo index_info0;
  index_info0.id = 0;
  index_info0.table_id = 0;
  index_info0.name = "test_index_col_0";
  index_info0.table_name = "test_table";
  index_info0.columns.emplace_back(col_info0);
  index_info0.root_node_id = 0;
  index_info0.type = parser::ConstraintType::CONSTRAINT_PRIMARY;
  index_info0.comment = "";

  IndexInfo index_info1;
  index_info1.id = 1;
  index_info1.table_id = 0;
  index_info1.name = "test_index_col_1";
  index_info1.table_name = "test_table";
  index_info1.columns.emplace_back(col_info1);
  index_info1.root_node_id = 1;
  index_info1.type = parser::ConstraintType::CONSTRAINT_INDEX;
  index_info1.comment = "";

  IndexProto index_proto;
  index_info1.Serialize(&index_proto);
  AMDB_ASSERT_EQ(index_info1.id, index_proto.id());
  AMDB_ASSERT_EQ(index_info1.name, index_proto.name());
  AMDB_ASSERT_EQ(index_info1.columns[0].id, index_proto.columns(0).id());
  AMDB_ASSERT_EQ(index_info1.columns[0].name, index_proto.columns(0).name());

  IndexInfo index_info1_tmp;
  index_info1_tmp.Deserialize(index_proto);
  AMDB_ASSERT_EQ(index_info1.id, index_info1_tmp.id);
  AMDB_ASSERT_EQ(index_info1.name, index_info1_tmp.name);
  AMDB_ASSERT_EQ(index_info1.type, index_info1_tmp.type);
  AMDB_ASSERT_EQ(index_info1.columns[0].id, index_info1_tmp.columns[0].id);
  AMDB_ASSERT_EQ(index_info1.columns[0].name, index_info1_tmp.columns[0].name);

  //-------------------------------------------

  TableInfo table_info;
  table_info.id = 0;
  table_info.name = "test_table";
  table_info.db_id = 0;
  table_info.db_name = "test_db";
  table_info.create_ts = 1999;
  table_info.update_ts = 2022;

  table_info.column_list.emplace_back(col_info0);
  table_info.column_list.emplace_back(col_info1);
  table_info.id_to_column.emplace(0, &col_info0);
  table_info.id_to_column.emplace(1, &col_info1);
  table_info.name_to_column.emplace("id", &col_info0);
  table_info.name_to_column.emplace("name", &col_info1);
  table_info.index_list.emplace_back(index_info0);
  table_info.index_list.emplace_back(index_info1);
  table_info.primary_index = &index_info0;
  table_info.id_to_index.emplace(1, &index_info1);

  TableProto table_proto;
  table_info.Serialize(&table_proto);
  AMDB_ASSERT_EQ(table_info.id, table_proto.id());
  AMDB_ASSERT_EQ(table_info.name, table_proto.name());
  AMDB_ASSERT_EQ(table_info.column_list[0].id, table_proto.columns(0).id());
  AMDB_ASSERT_EQ(table_info.primary_index->id, table_proto.row_index().id());

  TableInfo table_info_tmp;
  table_info_tmp.Deserialize(table_proto);
  AMDB_ASSERT_EQ(table_info.id, table_info_tmp.id);
  AMDB_ASSERT_EQ(table_info.name, table_info_tmp.name);
  AMDB_ASSERT_EQ(table_info.column_list[0].id,
                 table_info_tmp.column_list[0].id);
  AMDB_ASSERT_EQ(table_info.primary_index->id,
                 table_info_tmp.primary_index->id);

  //-------------------------------------------

  DatabaseInfo db_info;
  db_info.id = 0;
  db_info.name = "test_db";
  db_info.table_ids.insert(0);
  db_info.create_ts = 1999;
  db_info.update_ts = 2022;

  DatabaseProto db_proto;
  db_info.Serialize(&db_proto);
  AMDB_ASSERT_EQ(db_info.id, db_proto.id());
  AMDB_ASSERT_EQ(db_info.name, db_proto.name());
  AMDB_ASSERT_EQ(*db_info.table_ids.begin(), *db_proto.table_ids().begin());

  DatabaseInfo db_info_tmp;
  db_info_tmp.Deserialize(db_proto);
  AMDB_ASSERT_EQ(db_info.id, db_info_tmp.id);
  AMDB_ASSERT_EQ(db_info.name, db_info_tmp.name);
  AMDB_ASSERT_EQ(*db_info.table_ids.begin(), *db_info_tmp.table_ids.begin());
}
}  // namespace schema
}  // namespace amdb