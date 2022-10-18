#include "sql/storage/index.h"

#include "sql/chunk/iterator.h"
#include "sql/codec/rccodec.h"

namespace amdb {
namespace storage {

Index::Index(Arena *arena, schema::TableInfo *table_info,
             schema::IndexInfo *index_info, BptNonLeafNodeProto *root,
             KvStorageAPI *api) {
  tree_ctx_ = arena->CreateObject<TreeCtx>(arena, api);
  table_info_ = table_info;
  index_info_ = index_info;
  kv_api_ = api;
  bptree_ = tree_ctx_->AllocMem()->CreateObject<Bptree>(tree_ctx_, root);
}

Status Index::Save() {
  std::vector<std::string> keys;
  std::vector<std::string> values;
  tree_ctx_->PullUnsavedTreeNode(&keys, &values);
  Status status = kv_api_->MPutKV(keys, values);
  return status;
}

Status Index::Insert(chunk::Chunk *chunk) {
  chunk::Iterator itr(chunk);

  std::vector<std::string> encoded_keys;
  encoded_keys.reserve(chunk->Size());
  std::vector<std::string> encoded_values;
  encoded_values.reserve(chunk->Size());

  for (itr.Open(); itr.HasNext(); itr.Next()) {
    chunk::Row *row = itr.GetRow();
    std::string key;
    std::string value;
    codec::EncodeIndex(table_info_, index_info_, row, &key, &value);
    encoded_keys.emplace_back(std::move(key));
    encoded_values.emplace_back(value);
  }

  for (int i = 0; i < encoded_keys.size() && encoded_values.size(); i++) {
    Status status = bptree_->Insert(std::move(encoded_keys.at(i)),
                                    std::move(encoded_values.at(i)));
    if (status != Status::C_OK) {
      return status;
    }
  }
  return Status::C_OK;
}

Status Index::Delete(chunk::Chunk *chunk) {
  chunk::Iterator itr(chunk);

  std::vector<std::string> encoded_keys;
  encoded_keys.reserve(chunk->Size());

  for (itr.Open(); itr.HasNext(); itr.Next()) {
    chunk::Row *row = itr.GetRow();
    std::string key;
    std::string value;
    codec::EncodeIndex(table_info_, index_info_, row, &key, &value);
    encoded_keys.emplace_back(std::move(key));
  }

  for (int i = 0; i < encoded_keys.size(); i++) {
    Status status = bptree_->Delete(std::move(encoded_keys.at(i)));
    if (status != Status::C_OK) {
      return status;
    }
  }

  return Status::C_OK;
}

}  // namespace storage
}  // namespace amdb