#include "sql/storage/table.h"

#include "sql/chunk/iterator.h"
#include "sql/codec/rccodec.h"

namespace amdb {
namespace storage {
Table::Table(Arena *arena, schema::TableInfo *table_info, KvStorageAPI *api) {
  tree_ctx_ = arena->CreateObject<TreeCtx>(arena, api);
  kv_api_ = api;
  table_info_ = table_info;
}

Status Table::Prepare() { return Status::C_OK; }

Status Table::Submit() {
  Status status = row_index_->Save();
  RETURN_ERR_NOT_OK(status);
  // TODO sync -> async
  for (const auto &index : col_index_) {
    status = index.second->Save();
    RETURN_ERR_NOT_OK(status);
  }
  return Status::C_OK;
}

Status Table::Insert(chunk::Chunk *chunk) {
  Status status = Prepare();
  RETURN_ERR_NOT_OK(status);
  status = insertCore(chunk);
  RETURN_ERR_NOT_OK(status);
  status = Submit();
  RETURN_ERR_NOT_OK(status);
  return C_OK;
}

Status Table::Delete(chunk::Chunk *chunk) {
  Status status = Prepare();
  RETURN_ERR_NOT_OK(status);
  status = deleteCore(chunk);
  RETURN_ERR_NOT_OK(status);
  status = Submit();
  RETURN_ERR_NOT_OK(status);

  return Status::C_OK;
}

Status Table::Update(chunk::Chunk *old_chunk, chunk::Chunk *new_chunk) {
  Status status = Prepare();
  RETURN_ERR_NOT_OK(status);
  status = updateCore(old_chunk, new_chunk);
  RETURN_ERR_NOT_OK(status);
  status = Submit();
  RETURN_ERR_NOT_OK(status);

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

}  // namespace storage
}  // namespace amdb