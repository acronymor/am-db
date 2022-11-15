#include "sql/storage/table_iterator.h"

#include "sql/codec/rc_codec.h"

namespace amdb {
namespace storage {
Status TableIterator::GetItem(chunk::Chunk* chunk) {
  Status status = BaseIterator::Code();
  if (status != Status::C_OK && status != Status::C_BPTREE_ITERATOR_END) {
    return status;
  }

  if (data_segment_.empty()) {
    DEBUG("data_segment_ is empty");
    return Status::C_OK;
  }

  std::vector<std::string> keys;
  keys.reserve(data_segment_.size());
  schema::TableInfo* table_info = index_->table_info_;

  for (std::pair<std::string, std::string>& segment : data_segment_) {
    chunk::Row row(index_->TreeCtxx()->AllocMem(), chunk->GetRowDesc());
    codec::DecodeIndex(table_info, index_->index_info_, &segment.first,
                       &segment.second, &row);

    std::string key;
    // codec::EncodeRow(index_->table_info_, &row, &key, &value);
    for (schema::ColumnInfo& col_info : table_info->primary_index->columns) {
      std::string col_key = row.GetColValue(row.desc->ID(), col_info.id).ToString();
      codec::EncodeDataKey(table_info->db_id, table_info->id, col_key, &key);
    }

    keys.emplace_back(std::move(key));
  }

  std::vector<std::string> values;
  status = index_->GetRecords(keys, &values);
  RETURN_ERR_NOT_OK(status);

  status = chunk->PullData(index_->table_info_, values);

  visited_kv_count_ += data_segment_.size();
  return status;
}
}  // namespace storage
}  // namespace amdb