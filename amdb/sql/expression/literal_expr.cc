#include "sql/expression/literal_expr.h"

#include "sql/codec/codec.h"

namespace amdb {
namespace expr {
std::string LiteralExpr::ToString() { return value_.ToString(); }

const std::string& LiteralExpr::HashCode() {
  if (!hash_code_.empty()) {
    return hash_code_;
  }

  codec::EncodeUInt8(ExprNodeType::Literal, &hash_code_);
  if (value_.Type() == ExprValueType::Null) {
    return hash_code_;
  }

  codec::EncodeExprValue(value_, &hash_code_);

  return hash_code_;
}
}  // namespace expr
}  // namespace amdb