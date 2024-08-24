#pragma once

#include <cmath>

#include "sql/common/cmp.h"
#include "sql/expression/expr_value.h"
#include "sql/storage/data_cmp.h"

namespace amdb {
namespace expr {
inline common::CmpOpt ExprValueCmp(const expr::ExprValue* v1,
                                   const expr::ExprValue* v2) {
#define COMPARE_INT_TYPE(LHS, RHS, TYPE, VAL)           \
  do {                                                  \
    if ((TYPE)(LHS).u.VAL < (TYPE)(RHS).u.VAL) {        \
      return common::CmpOpt::lt;                        \
    } else if ((TYPE)(LHS).u.VAL > (TYPE)(RHS).u.VAL) { \
      return common::CmpOpt::gt;                        \
    }                                                   \
    return common::CmpOpt::eq;                          \
  } while (0);

#define COMPARE_DECIMAL_TYPE(V1, V2, TYPE)                    \
  do {                                                        \
    TYPE epsilon = std::numeric_limits<TYPE>::epsilon() * 2;  \
    if (std::fabs(V1 - V2) < epsilon) {                       \
      return common::CmpOpt::eq;                              \
    }                                                         \
    return V1 < V2 ? common::CmpOpt::lt : common::CmpOpt::gt; \
  } while (0);

  if (v1->Type() == v2->Type()) {
    switch (v1->Type()) {
      case ExprValueType::Bool:
      case ExprValueType::UInt8:
        COMPARE_INT_TYPE(*v1, *v2, uint8_t, uint8_value);
      case ExprValueType::Int8:
        COMPARE_INT_TYPE(*v1, *v2, int8_t, uint8_value);
      case ExprValueType::UInt32:
        COMPARE_INT_TYPE(*v1, *v2, uint32_t, uint32_value);
      case ExprValueType::Int32:
        COMPARE_INT_TYPE(*v1, *v2, int32_t, uint32_value);
      case ExprValueType::UInt64:
        COMPARE_INT_TYPE(*v1, *v2, uint64_t, uint64_value);
      case ExprValueType::Int64:
        COMPARE_INT_TYPE(*v1, *v2, int64_t, uint64_value);
      case ExprValueType::Float32:
        COMPARE_DECIMAL_TYPE(v1->FloatValue(), v2->FloatValue(), float);
      case ExprValueType::Double:
        COMPARE_DECIMAL_TYPE(v1->DoubleValue(), v2->DoubleValue(), double);
      case ExprValueType::String:
        return storage::DataCmp(v1->StringValue(), v2->StringValue());
      case ExprValueType::Invalid:
      case ExprValueType::MinInf:
      case ExprValueType::MaxInf:
        return common::CmpOpt::eq;
    }
  }

#undef COMPARE_DECIMAL_TYPE
#undef COMPARE_INT_TYPE

  return common::CmpOpt::ne;
}

inline common::CmpOpt ExprValuesCmp(const std::vector<expr::ExprValue>& v1,
                                    const std::vector<expr::ExprValue>& v2) {
  for (size_t i = 0; i < v1.size() && i < v2.size(); i++) {
    common::CmpOpt opt = ExprValueCmp(&v1.at(i), &v2.at(i));
    if (common::IsCmpNe(opt)) {
      return opt;
    }
  }

  if (v1.size() == v2.size()) {
    return common::CmpOpt::eq;
  }

  return v1.size() > v2.size() ? common::CmpOpt::lt : common::CmpOpt::gt;
}

using ExprValueSortCmpFunc = int(*)(const ExprValue&, const ExprValue&);

struct ExprValueLess {
  ExprValueSortCmpFunc sorter_cmp = nullptr;

  ExprValueLess(ExprValueSortCmpFunc cmp) : sorter_cmp(cmp) {}
  bool operator()(const ExprValue& lval, const ExprValue& rval) const {
    return sorter_cmp(lval, rval) == common::CmpOpt::gt;
  }
};
}  // namespace expr
}  // namespace amdb