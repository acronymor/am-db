#pragma once

#include <iostream>

namespace amdb {
namespace common {
enum CmpOpt : unsigned char { lt = 1, le = 2, gt = 3, ge = 4, eq = 5, ne = 6 };

inline bool IsCmpLt(CmpOpt op) { return op == lt; }
inline bool IsCmpLe(CmpOpt op) { return op == lt || op == eq; }
inline bool IsCmpGt(CmpOpt op) { return op == gt; }
inline bool IsCmpGe(CmpOpt op) { return op == gt || op == eq; }
inline bool IsCmpEq(CmpOpt op) { return op == eq; }
inline bool IsCmpNe(CmpOpt op) { return !IsCmpEq(op); }
}  // namespace common
}  // namespace amdb