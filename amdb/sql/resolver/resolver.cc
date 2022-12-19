#include "sql/resolver/resolver.h"

#include "sql/expression/literal_expr.h"

namespace amdb {
namespace resolver {
expr::ExprNode* Resolver::ExprNode(parser::ExprNode* p_node) {
  expr::ExprNode* node = nullptr;

  switch (p_node->expr_type) {
    case parser::ET_COLUMN: {
      parser::ColumnName* col_name = dynamic_cast<parser::ColumnName*>(p_node);
      std::cout << col_name->name << std::endl;
    }; break;
    case parser::ET_LITETAL: {
      parser::LiteralExpr* literal_expr = dynamic_cast<parser::LiteralExpr*>(p_node);
      std::cout << literal_expr->to_string() << std::endl;

      node = stmt_ctx_->arena->CreateObject<expr::LiteralExpr>();
    }; break;
    case parser::ET_FUNC: {
      for (size_t i = 0; i < p_node->children.size(); i++) {
        ExprFunc(dynamic_cast<parser::FuncExpr*>(p_node->children[i]));
      }
    }; break;
    default:
      break;
  }

  return node;
}

expr::ExprNode* Resolver::ExprFunc(parser::FuncExpr* p_node) {
  expr::ExprNode* node = nullptr;

  switch (p_node->func_type) {
    case parser::FT_LOGIC_AND: {
    }; break;
    case parser::FT_GT: {
      std::cout << p_node->func_type << std::endl;
      for (size_t i = 0; i < p_node->children.size(); i++) {
        node = ExprNode(dynamic_cast<parser::ExprNode*>(p_node->children[i]));
      }
    }; break;
  }

  return node;
}
}  // namespace resolver
}  // namespace amdb
