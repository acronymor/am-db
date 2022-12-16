#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "sql/common/statuscode.h"

namespace amdb {
namespace storage {
enum struct IDType : uint8_t {
  Database = 0,
  Table = 1,
  Column = 2,
  Unknown = 3,
};

class IdAllocator {
 public:
  explicit IdAllocator(uint32_t cache_size) : bucket_capacity_(cache_size) {
    id_info_.emplace(IDType::Database, std::vector<AllocInfo>(bucket_capacity_));
    id_info_.emplace(IDType::Table, std::vector<AllocInfo>(bucket_capacity_));
    id_info_.emplace(IDType::Column, std::vector<AllocInfo>(bucket_capacity_));
  }
  ~IdAllocator() = default;

  Status AllocateID(IDType type, const std::string& key, uint64_t* id);

  Status BatchAllocateID(IDType type, const std::string& key, size_t batch_size, std::vector<uint64_t>* id_list);

  struct AllocInfo {
    uint64_t cur_id;
    uint64_t max_id;
    size_t left_num;
    AllocInfo() : cur_id(0), max_id(0), left_num(0) {}
  };

 private:
  std::unordered_map<IDType, std::vector<AllocInfo>> id_info_;
  uint32_t bucket_capacity_;
};

struct IdAllocatorApiOptions {};

class IdAllocatorSingleton {
 public:
  [[nodiscard]] static Status Init(const IdAllocatorApiOptions& options);
  static IdAllocator* GetInstance();

 private:
  static IdAllocator* instance_;
};
}  // namespace storage
}  // namespace amdb