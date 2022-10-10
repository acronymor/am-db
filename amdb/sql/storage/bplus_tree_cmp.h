#pragma once

#include <cstring>
#include <iostream>
#include <unordered_map>

namespace amdb {
namespace storage {

constexpr int kCmpResNeq = -0xfe;
constexpr int kCmpResNull = -0xff;
constexpr int kCmpResLt = -1;
constexpr int kCmpResEq = 0;
constexpr int kCmpResGt = 1;
inline bool IsCmpLt(int res) { return res == kCmpResLt; }
inline bool IsCmpLe(int res) { return res == kCmpResLt || res == kCmpResEq; }
inline bool IsCmpGt(int res) { return res == kCmpResGt; }
inline bool IsCmpGe(int res) { return res == kCmpResGt || res == kCmpResEq; }
inline bool IsCmpEq(int res) { return res == kCmpResEq; }

inline int DefaultIndexDataCmpFunc(const char* data1, size_t len1,
                                   const char* data2, size_t len2) {
  const char* p1 = data1;
  const char* p2 = data2;
  size_t min_len = std::min(len1, len2);
  if (min_len > 0) {
    int r = std::memcmp(p1, p2, min_len);
    if (r < 0) {
      return kCmpResLt;
    } else if (r > 0) {
      return kCmpResGt;
    }
  }
  if (len1 < len2) {
    return kCmpResLt;
  } else if (len1 > len2) {
    return kCmpResGt;
  }
  return kCmpResEq;
}

inline bool GreaterThan(const std::string& one, const std::string& another,
                        bool is_gt) {
  int cmp_res = DefaultIndexDataCmpFunc(one.data(), one.size(), another.data(),
                                        another.size());
  return is_gt ? IsCmpGt(cmp_res) : IsCmpGe(cmp_res);
}

inline bool LessThan(const std::string& one, const std::string& another,
                     bool is_lt) {
  int cmp_res = DefaultIndexDataCmpFunc(one.data(), one.size(), another.data(),
                                        another.size());
  return is_lt ? IsCmpLt(cmp_res) : IsCmpLe(cmp_res);
}

}  // namespace storage
}  // namespace amdb