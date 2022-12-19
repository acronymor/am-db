#include "sql/expression/expr_node.h"
#include "sql/parser/parser.h"

namespace amdb {
namespace resolver {
class Resolver {
 public:
  explicit Resolver(StatementContext* stmt_ctx) : stmt_ctx_(stmt_ctx){};

  expr::ExprNode* ExprNode(parser::ExprNode* p_node);

  expr::ExprNode* ExprFunc(parser::FuncExpr* p_node);

 private:
  StatementContext* stmt_ctx_;
};
}  // namespace resolver
}  // namespace amdb