#include "sql/storage/table.h"

#include "sql/chunk/iterator.h"
#include "sql/codec/rc_codec.h"

namespace amdb {
namespace storage {
Table::Table(Arena *arena, schema::TableInfo *table_info) {
  kv_api_ = KvStorageAPISingleton::GetInstance();
  table_info_ = table_info;
  arena_ = arena;
  metadata_api_ = arena_->CreateObject<Metadata>(kv_api_);
}

Status Table::Prepare() {
  RETURN_ERR_NOT_OK(loadMeta());
  return Status::C_OK;
}

Status Table::Submit() {
  // TODO sync -> async
  for (const auto &index : col_index) {
    Status status = index.second->Save();
    RETURN_ERR_NOT_OK(status);
  }

  /*
  RETURN_ERR_NOT_OK(saveMeta());
   */
  return Status::C_OK;
}

Status Table::Insert(chunk::Chunk *chunk) {
  RETURN_ERR_NOT_OK(Prepare());
  RETURN_ERR_NOT_OK(insertCore(chunk));
  RETURN_ERR_NOT_OK(Submit());
  return C_OK;
}

Status Table::Delete(chunk::Chunk *chunk) {
  RETURN_ERR_NOT_OK(Prepare());
  RETURN_ERR_NOT_OK(deleteCore(chunk));
  RETURN_ERR_NOT_OK(Submit());

  return Status::C_OK;
}

Status Table::Update(chunk::Chunk *old_chunk, chunk::Chunk *new_chunk) {
  RETURN_ERR_NOT_OK(Prepare());
  RETURN_ERR_NOT_OK(updateCore(old_chunk, new_chunk));
  RETURN_ERR_NOT_OK(Submit());

  return Status::C_OK;
}

Status Table::insertCore(chunk::Chunk *chunk) {
  std::vector<std::string> encoded_keys;
  encoded_keys.reserve(chunk->Size());
  std::vector<std::string> encoded_values;
  encoded_values.reserve(chunk->Size());

  chunk::Iterator itr(chunk);
  for (itr.Open(); itr.HasNext(); itr.Next()) {
    chunk::Row *row = itr.GetRow();
    std::string key;
    std::string value;
    codec::EncodeRow(table_info_, row, &key, &value);
    encoded_keys.emplace_back(std::move(key));
    encoded_values.emplace_back(std::move(value));
  }

  // save data
  Status status = kv_api_->MPutKV(encoded_keys, encoded_values);
  RETURN_ERR_NOT_OK(status);

  // TODO sync -> async
  // save index which contains primary index
  for (const auto &index : col_index) {
    status = index.second->Insert(chunk);
    RETURN_ERR_NOT_OK(status);
  }

  return Status::C_OK;
}

Status Table::deleteCore(chunk::Chunk *chunk) {
  // TODO sync -> async
  for (const auto &index : col_index) {
    Status status = index.second->Delete(chunk);
    RETURN_ERR_NOT_OK(status);
  }

  std::vector<std::string> encoded_keys;
  encoded_keys.reserve(chunk->Size());

  chunk::Iterator itr(chunk);
  for (itr.Open(); itr.HasNext(); itr.Next()) {
    chunk::Row *row = itr.GetRow();
    std::string key;
    std::string value;
    codec::EncodeRow(table_info_, row, &key, &value);
    encoded_keys.emplace_back(std::move(key));
  }

  Status status = kv_api_->MDelKV(encoded_keys);
  RETURN_ERR_NOT_OK(status);

  return Status::C_OK;
}

Status Table::updateCore(chunk::Chunk *old_chunk, chunk::Chunk *new_chunk) {
  return Status::C_OK;
}

Status Table::loadMeta() {
  auto get_index = [this](schema::IndexInfo *index_info) -> Index * {
    BptNodeProto index_tree_root;
    Status status = metadata_api_->LoadTreeNode(
        table_info_->db_id, table_info_->id, index_info->id,
        index_info->root_node_id, &index_tree_root);

    if (status == Status::C_STORAGE_KV_NOT_FOUND) {
      index_tree_root.set_id(index_info->root_node_id);
    }
    return arena_->CreateObject<Index>(kv_api_, arena_, table_info_, index_info,
                                       &index_tree_root);
  };

  for (auto &index : table_info_->index_list) {
    Index* idx = get_index(&index);
    col_index.emplace(index.id, idx);
    if (index.type == schema::CONSTRAINT_PRIMARY) {
      row_index = idx;
    }
  }

  return Status::C_OK;
}

Status Table::saveMeta() {
  Status status = metadata_api_->DumpTableMeta(table_info_->db_id,
                                               table_info_->id, table_info_);
  RETURN_ERR_NOT_OK(status);

  for (const auto &index : table_info_->id_to_index) {
    status = metadata_api_->DumpIndexMeta(table_info_->db_id, table_info_->id,
                                          index.second);
    RETURN_ERR_NOT_OK(status);
  }

  return Status::C_OK;
}

}  // namespace storage
}  // namespace amdb