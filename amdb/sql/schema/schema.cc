#include "sql/schema/schema.h"

namespace amdb {
namespace schema {
Status ColumnInfo::Serialize(ColumnProto *output) {
  output->set_id(id);
  output->set_name(name);
  output->set_table_id(table_id);
  output->set_table_name(table_name);
  output->set_comment(comment);
  return Status::C_OK;
}

Status ColumnInfo::Deserialize(const ColumnProto &input) {
  id = input.id();
  name = input.name();
  table_id = input.table_id();
  table_name = input.table_name();
  comment = input.comment();
  return Status::C_OK;
}

Status IndexInfo::Serialize(IndexProto *output) {
  output->set_id(id);
  output->set_table_id(table_id);
  output->set_name(name);
  output->set_table_name(table_name);
  output->set_root_node_id(root_node_id);
  output->set_type(type);
  output->set_comment(comment);

  for (ColumnInfo &column_info : columns) {
    ColumnProto col_proto;
    column_info.Serialize(&col_proto);
    *output->add_columns() = std::move(col_proto);
  }

  return Status::C_OK;
}

Status IndexInfo::Deserialize(const IndexProto &input) {
  id = input.id();
  table_id = input.table_id();
  name = input.name();
  table_name = input.table_name();
  root_node_id = input.root_node_id();
  type = (ConstraintType)input.type();
  comment = input.comment();

  columns.reserve(input.columns_size());
  for (const auto &column : input.columns()) {
    columns.emplace_back();
    columns.back().Deserialize(column);
  }

  return Status::C_OK;
}

Status TableInfo::Serialize(TableProto *output) {
  output->set_id(id);
  output->set_database_id(db_id);
  output->set_name(name);
  output->set_database_name(db_name);
  output->set_create_ts(create_ts);
  output->set_update_ts(update_ts);

  for (ColumnInfo &column : column_list) {
    ColumnProto col_proto;
    column.Serialize(&col_proto);
    *output->add_columns() = std::move(col_proto);
  }

  for (IndexInfo &index : index_list) {
    IndexProto col_index_proto;
    index.Serialize(&col_index_proto);
    *output->add_col_index() = std::move(col_index_proto);
  }

  return Status::C_OK;
}

Status TableInfo::Deserialize(const TableProto &input) {
  id = input.id();
  db_id = input.database_id();
  name = input.name();
  db_name = input.database_name();
  create_ts = input.create_ts();
  update_ts = input.update_ts();

  column_list.reserve(input.columns_size());

  for (const auto &column : input.columns()) {
    column_list.emplace_back();
    ColumnInfo &column_info = column_list.back();
    column_info.Deserialize(column);
    id_to_column[column_info.id] = &column_info;
    name_to_column[column_info.name] = &column_info;
  }

  index_list.reserve(input.col_index_size());
  id_to_index.reserve(input.col_index_size());
  name_to_index.reserve(input.col_index_size());

  for (const auto &index : input.col_index()) {
    index_list.emplace_back();
    IndexInfo &index_info = index_list.back();
    index_info.Deserialize(index);

    if (index_info.type == ConstraintType::CONSTRAINT_PRIMARY) {
      primary_index = &index_info;
    }

    id_to_index[index.id()] = &index_info;
    name_to_index[index.name()] = &index_info;
  }

  for (IndexInfo &index : index_list) {
    for (const ColumnInfo &column : index.columns) {
      column_id_to_index[column.id].emplace_back(&index);
    }
  }

  return Status::C_OK;
}

Status DatabaseInfo::Serialize(DatabaseProto *output) {
  output->set_id(id);
  output->set_name(name);
  output->set_create_ts(create_ts);
  output->set_update_ts(update_ts);
  output->set_comment(comment);

  for (const uint64_t &id : table_ids) {
    output->add_table_ids(id);
  }
  return Status::C_OK;
}

Status DatabaseInfo::Deserialize(const DatabaseProto &input) {
  id = input.id();
  name = input.name();
  create_ts = input.create_ts();
  update_ts = input.update_ts();
  comment = input.comment();

  for (const uint64_t &id : input.table_ids()) {
    table_ids.insert(id);
  }
  return Status::C_OK;
}
}  // namespace schema
}  // namespace amdb