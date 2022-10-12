#pragma once

#include <cstring>
#include <iostream>
#include <unordered_map>

namespace amdb {
namespace storage {

enum CmpOpt : unsigned char { lt = 1, le = 2, gt = 3, ge = 4, eq = 5, ne = 6 };

inline bool IsCmpLt(CmpOpt op) { return op == lt; }
inline bool IsCmpLe(CmpOpt op) { return op == lt || op == eq; }
inline bool IsCmpGt(CmpOpt op) { return op == gt; }
inline bool IsCmpGe(CmpOpt op) { return op == gt || op == eq; }
inline bool IsCmpEq(CmpOpt op) { return op == eq; }
inline bool IsCmpNe(CmpOpt op) { return !IsCmpEq(op); }

inline CmpOpt DataCmp(const std::string& s1, const std::string& s2) {
  int res = s1.compare(s2);
  CmpOpt opt = CmpOpt::eq;

  if (res < 0) {
    return CmpOpt::lt;
  } else if (res > 0) {
    return CmpOpt::gt;
  } else {
    return CmpOpt::eq;
  }
}
}  // namespace storage
}  // namespace amdb