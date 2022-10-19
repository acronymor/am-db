#include "sql/storage/table.h"

#include "sql/chunk/iterator.h"
#include "sql/codec/kv_codec.h"
#include "sql/codec/rc_codec.h"

namespace amdb {
namespace storage {
Table::Table(Arena *arena, schema::TableInfo *table_info, KvStorageAPI *api) {
  kv_api_ = api;
  table_info_ = table_info;
  arena_ = arena;
}

Status Table::Prepare() {
  RETURN_ERR_NOT_OK(loadMeta());
  return Status::C_OK;
}

Status Table::Submit() {
  Status status = row_index_->Save();
  RETURN_ERR_NOT_OK(status);
  // TODO sync -> async
  for (const auto &index : col_index_) {
    status = index.second->Save();
    RETURN_ERR_NOT_OK(status);
  }

  RETURN_ERR_NOT_OK(saveMeta());
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
  // save other index
  for (const auto &index : col_index_) {
    status = index.second->Insert(chunk);
    RETURN_ERR_NOT_OK(status);
  }

  // save primary index
  status = row_index_->Insert(chunk);
  RETURN_ERR_NOT_OK(status);

  return Status::C_OK;
}

Status Table::deleteCore(chunk::Chunk *chunk) {
  // delete primary index
  Status status = row_index_->Delete(chunk);
  RETURN_ERR_NOT_OK(status);

  // TODO sync -> async
  // delete other index
  for (const auto &index : col_index_) {
    status = index.second->Delete(chunk);
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

  // delete data
  status = kv_api_->MDelKV(encoded_keys);
  RETURN_ERR_NOT_OK(status);

  return Status::C_OK;
}

Status Table::updateCore(chunk::Chunk *old_chunk, chunk::Chunk *new_chunk) {
  return Status::C_OK;
}

Status Table::loadMeta() {
  auto get_index = [this](const schema::IndexInfo *index_info,
                          Index *index) -> Status {
    std::string key;
    codec::EncodeTreeNodeKey(table_info_->db_id, table_info_->id,
                             index_info->id, 0, &key);
    std::string value;
    Status status = kv_api_->GetKV(key, &value);
    RETURN_ERR_NOT_OK(status);

    std::string str;
    index->TreeRoot()->Deserialize(str);

    return Status::C_OK;
  };

  Status status = get_index(table_info_->primary_index, row_index_);
  RETURN_ERR_NOT_OK(status);

  for (const auto &entry : table_info_->id_to_index) {
    Index *index;
    status = get_index(entry.second, index);
    RETURN_ERR_NOT_OK(status);

    col_index_.emplace(entry.first, index);
  }

  return Status::C_OK;
}

Status Table::saveMeta() {
  auto get_index = [this](const schema::IndexInfo *index_info, Index *index,
                          std::string *key, std::string *value) -> Status {
    codec::EncodeTreeNodeKey(table_info_->db_id, table_info_->id,
                             table_info_->primary_index->id, 0, key);
    Status status = index->TreeRoot()->Serialize(value);
    return status;
  };

  std::unordered_map<std::string, std::string> meta;
  meta.reserve(col_index_.size() + 1);

  std::string row_key, row_value;
  get_index(table_info_->primary_index, row_index_, &row_key, &row_value);
  meta.emplace(row_key, row_value);

  for (const auto &entry : col_index_) {
    std::string col_key, col_value;
    schema::IndexInfo *index_info = table_info_->id_to_index.at(entry.first);
    get_index(index_info, entry.second, &col_key, &col_value);
    meta.emplace(col_key, col_value);
  }

  Status status = kv_api_->MPutKV(meta);
  RETURN_ERR_NOT_OK(status);

  return Status::C_OK;
}

}  // namespace storage
}  // namespace amdb