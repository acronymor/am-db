#include "sql/analyzer/expr_analyzer.h"

namespace amdb {
namespace analyzer {
expr::ExprNode* ExprAnalyzer::AnalyzeNode(parser::ExprNode* parse_node) {
  return toExprNode(parse_node);
}

Status ExprAnalyzer::AnalyzeWhere(parser::ExprNode* parse_node, std::vector<expr::ExprNode*> expr_node) { return C_OK; }

Status ExprAnalyzer::AnalyzeBetween(parser::FuncExpr* func_expr, expr::ExprNode* scala_expr) { return Status::C_OK; }

expr::ExprNode* ExprAnalyzer::toExprNode(parser::ExprNode* parse_expr) {
  expr::ExprNode* expr_node{ nullptr};

  switch (parse_expr->expr_type) {
    case parser::ExprType::ET_LITETAL: {
      expr_node = toLiteralNode(dynamic_cast<parser::LiteralExpr*>(parse_expr));
    }; break;

    case parser::ExprType::ET_COLUMN: {
      // expr_node = stmt_ctx_->arena->CreateObject<expr::LiteralExpr>();
    }; break;

    case parser::ExprType::ET_FUNC: {
      //
    }; break;
    default:
      ERROR("ERROR expr type");
  }

  return expr_node;
}

expr::LiteralExpr* ExprAnalyzer::toLiteralNode(parser::LiteralExpr* parser_node) {
  expr::LiteralExpr* expr_node = nullptr;

  switch (parser_node->literal_type) {
    case parser::LT_INT: {
      expr::ExprValue value = expr::ExprValue::NewInt64(parser_node->_u.int64_val);
      expr_node = stmt_ctx_->arena->CreateObject<expr::LiteralExpr>(value);
    }; break;

    case parser::LT_DOUBLE: {
      expr::ExprValue value = expr::ExprValue::NewDouble(parser_node->_u.double_val);
      expr_node = stmt_ctx_->arena->CreateObject<expr::LiteralExpr>(value);
    }; break;

    case parser::LT_BOOL: {
      expr::ExprValue value = expr::ExprValue::NewBool(parser_node->_u.bool_val);
      expr_node = stmt_ctx_->arena->CreateObject<expr::LiteralExpr>(value);
    }; break;

    case parser::LT_NULL: {
      expr::ExprValue value = expr::ExprValue::NewNull();
      expr_node = stmt_ctx_->arena->CreateObject<expr::LiteralExpr>(value);
    }; break;

    default:
      break;
  }
  return expr_node;
}

Status ExprAnalyzer::toFuncExprNode(parser::FuncExpr* parse_func, expr::ExprNode* expr_node) { return Status::C_OK; }

Status ExprAnalyzer::toScalaFuncNode(parser::FuncExpr* parse_func, expr::ExprNode* expr_node) { return Status::C_OK; }

Status ExprAnalyzer::toAggFuncNode(parser::FuncExpr* parse_func, expr::ExprNode* expr_node) { return Status::C_OK; }

}  // namespace analyzer
}  // namespace amdb
