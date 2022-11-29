#pragma once

#include <iostream>
#include "common/assert.h"
#include "sql/common/statuscode.h"
#include "sql/expression/expr_value.h"

namespace amdb {
struct StatementContext;

namespace expr {
enum ExprNodeType { Literal = 1 };

class ExprNode {
 public:
  ExprNode() : constant_(true) {}
  ExprNode(ExprNodeType type, bool constant)
      : node_type_(type), constant_(constant) {}

  virtual ~ExprNode() = default;

  void AddChild(ExprNode* child);

  virtual Status Open(StatementContext* ctx);
  virtual Status Close(StatementContext* ctx);

  virtual const std::string& HashCode() = 0;
  virtual std::string ToString() = 0;

 protected:
  ExprNode* parent_ = nullptr;
  std::vector<ExprNode*> children_;

  bool constant_ = false;

  ExprNodeType node_type_ = ExprNodeType::Literal;
  ExprValueType eval_type_ = ExprValueType::Invalid;

  std::string hash_code_;
};
}  // namespace expr
}  // namespace amdb