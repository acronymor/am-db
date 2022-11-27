#pragma once

#include "sql/common/cmp.h"

namespace amdb {
namespace storage {
inline common::CmpOpt DataCmp(const std::string& s1, const std::string& s2) {
  int res = s1.compare(s2);
  common::CmpOpt opt = common::CmpOpt::ne;

  if (res < 0) {
    opt = common::CmpOpt::lt;
  } else if (res > 0) {
    opt = common::CmpOpt::gt;
  } else {
    opt = common::CmpOpt::eq;
  }

  return opt;
}
}  // namespace storage
}  // namespace amdb