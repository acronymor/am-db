#pragma once

#include "sql/common/cmp.h"
#include "sql/expression/expr_cmp.h"
#include "sql/planner/range.h"

namespace amdb {
namespace planner {
inline common::CmpOpt RangePointCmp(const IndexRange::RangePoint* r1,
                                    const IndexRange::RangePoint* r2) {
  return expr::ExprValuesCmp(r1->value, r2->value);
}

inline common::CmpOpt IndexRangeCmp(const IndexRange* r1,
                                    const IndexRange* r2) {
  common::CmpOpt opt = common::CmpOpt::ne;

  common::CmpOpt opt1 = RangePointCmp(&r1->lower, &r2->lower);
  if (common::IsCmpNe(opt1)) {
    return opt1;
  }

  common::CmpOpt opt2 = RangePointCmp(&r1->upper, &r2->upper);
  if (common::IsCmpNe(opt2)) {
    return opt2;
  }

  if (common::IsCmpEq(opt1) && common::IsCmpEq(opt2)) {
    opt = common::CmpOpt::eq;
  }

  return opt;
}
}  // namespace planner
}  // namespace amdb