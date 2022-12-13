#include "sql/expression/expr_node.h"

namespace amdb {
namespace expr {
class LiteralExpr : public ExprNode {
 public:
  LiteralExpr() : ExprNode(ExprNodeType::Literal, true), value_(ExprValue::NewNull()) { eval_type_ = value_.Type(); };

  LiteralExpr(const ExprValue& value) : ExprNode(ExprNodeType::Literal, true), value_(std::move(value)) {
    eval_type_ = value.Type();
  }

  const ExprValue& GetValue() { return value_; }

  std::string ToString() override;
  const std::string& HashCode() override;

 private:
  ExprValue value_;
};
}  // namespace expr
}  // namespace amdb