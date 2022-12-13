#include "sql/analyzer/analyzer.h"
#include "sql/expression/literal_expr.h"
#include "sql/parser/expr.h"

namespace amdb {
namespace analyzer {
class ExprAnalyzer : public AstAnalyzer {
 public:
  explicit ExprAnalyzer(StatementContext* stmt_ctx) : AstAnalyzer(stmt_ctx){};

  Status Analyze() override { return Status::C_ANALYZE_ERROR; };

  expr::ExprNode* AnalyzeNode(parser::ExprNode* parse_node);

  Status AnalyzeSelect(parser::ExprNode* parse_node, std::vector<expr::ExprNode*> expr_node);

  Status AnalyzeFrom(parser::ExprNode* parse_node, std::vector<expr::ExprNode*> expr_node);

  Status AnalyzeWhere(parser::ExprNode* parse_node, std::vector<expr::ExprNode*> expr_node);

  Status AnalyzeBetween(parser::FuncExpr* func_expr, expr::ExprNode* scala_expr);

 private:
  expr::ExprNode* toExprNode(parser::ExprNode* parse_expr);

  expr::LiteralExpr* toLiteralNode(parser::LiteralExpr* parser_node);

  Status toFuncExprNode(parser::FuncExpr* parse_func, expr::ExprNode* expr_node);

  Status toScalaFuncNode(parser::FuncExpr* parse_func, expr::ExprNode* expr_node);

  Status toAggFuncNode(parser::FuncExpr* parse_func, expr::ExprNode* expr_node);
};
}  // namespace analyzer
}  // namespace amdb