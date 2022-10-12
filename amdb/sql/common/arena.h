#pragma once

#include "google/protobuf/arena.h"

namespace amdb {
class MemTracker {
 public:
  explicit MemTracker() = default;

  uint64_t CurrentUsage() { return cur_usage_.load(std::memory_order_relaxed); }

  uint64_t AddUsage(uint64_t use) {
    uint64_t cur = cur_usage_.fetch_add(use, std::memory_order_relaxed);
    return cur + use;
  }

  uint64_t SubUsage(uint64_t use) {
    uint64_t cur = cur_usage_.fetch_sub(use, std::memory_order_relaxed);
    return cur - use;
  }

 private:
  std::atomic<uint64_t> cur_usage_{0};
};

class Arena {
 public:
  explicit Arena(MemTracker* tracker = nullptr) : tracker_(tracker){};

  virtual ~Arena() {
    if (tracker_ != nullptr) {
      tracker_->SubUsage(tracker_->CurrentUsage());
    }
    delete tracker_;
  }

  template <typename T, typename... Args>
  T* CreateObject(Args&&... args) {
    if (tracker_ != nullptr) {
      tracker_->AddUsage(sizeof(T));
    }
    return google::protobuf::Arena::Create<T>(&arena_,
                                              std::forward<Args>(args)...);
  }

  uint64_t SpaceAllocated() { return arena_.SpaceAllocated(); }

  uint64_t SpaceUsed() {
    if (tracker_ == nullptr) {
      return ((unsigned int)(-1)) >> 1;
    }
    return tracker_->CurrentUsage();
  }

 private:
  google::protobuf::Arena arena_;

  MemTracker* tracker_;
};
}  // namespace amdb