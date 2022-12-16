#include "sql/storage/id_allocator.h"

#include "common/assert.h"
#include "sql/codec/schema_codec.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {
namespace storage {
namespace {
void GenIncKey(IDType type, uint32_t bucket_idx, std::string* key) {
  switch (type) {
    case IDType::Database:
      codec::EncodeDatabaseAllocID(bucket_idx, key);
      break;
    case IDType::Table:
      codec::EncodeTableAllocID(bucket_idx, key);
      break;
    case IDType::Column:
      codec::EncodeColumnAllocID(bucket_idx, key);
      break;
    default:
      ERROR("Not Support type, ", type);
      break;
  }
}
}  // namespace

IdAllocator* IdAllocatorSingleton::instance_ = nullptr;

Status IdAllocatorSingleton::Init(const IdAllocatorApiOptions& options) {
  instance_ = new IdAllocator(1024);
  return instance_ == nullptr ? Status::C_STORAGE_ID_GENERATOR_ERROR : Status::C_OK;
}

IdAllocator* IdAllocatorSingleton::GetInstance() { return instance_; }

Status IdAllocator::AllocateID(IDType type, const std::string& key, uint64_t* id) {
  std::vector<uint64_t> ids(1, 0);
  Status status = BatchAllocateID(type, key, 1, &ids);
  *id = ids.at(0);
  return status;
}

Status IdAllocator::BatchAllocateID(IDType type, const std::string& key, size_t batch, std::vector<uint64_t>* ids) {
  uint32_t capacity = bucket_capacity_;
  ids->clear();

  auto id_alloc = [&batch, &capacity](AllocInfo& alloc_info, size_t bucket_idx, std::vector<uint64_t>* ids) -> void {
    while (alloc_info.left_num > 0 && ids->size() < batch) {
      ids->push_back(alloc_info.cur_id + bucket_idx + 1);
      alloc_info.cur_id += capacity;
      alloc_info.left_num--;
    }
  };

  std::vector<IdAllocator::AllocInfo> buckets = id_info_.at(type);
  size_t bucket_idx = (std::hash<std::string>{}(key) & (bucket_capacity_ - 1));
  AllocInfo& alloc_info = buckets.at(bucket_idx);

  size_t need_fetch_num = batch > alloc_info.left_num ? batch - alloc_info.left_num : 0;
  need_fetch_num = need_fetch_num > bucket_capacity_ ? bucket_capacity_ : need_fetch_num;

  if (need_fetch_num == 0) {
    id_alloc(alloc_info, bucket_idx, ids);
  }

  std::string inc_key;
  GenIncKey(type, bucket_idx, &inc_key);

  uint64_t step = need_fetch_num * bucket_capacity_;
  uint64_t new_max_id = 0;
  Status status = KvStorageAPISingleton::GetInstance()->Incrby(inc_key, step, &new_max_id);
  AMDB_ASSERT_EQ(Status::C_OK, status);

  alloc_info.cur_id = new_max_id - step;
  alloc_info.max_id = new_max_id;
  alloc_info.left_num = need_fetch_num;

  id_alloc(alloc_info, bucket_idx, ids);

  return C_OK;
}
}  // namespace storage
}  // namespace amdb