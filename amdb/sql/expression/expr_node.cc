#include "sql/expression/expr_node.h"

namespace amdb {
namespace expr {

Status ExprNode::Open(StatementContext *ctx) {
  for (auto ch : children_) {
    RETURN_ERR_NOT_OK(ch->Open(ctx));
  }
  return Status::C_OK;
}

Status ExprNode::Close(StatementContext *ctx) {
  for (auto ch : children_) {
    RETURN_ERR_NOT_OK(ch->Close(ctx));
  }
  return Status::C_OK;
}

void ExprNode::AddChild(ExprNode *child) {
  children_.push_back(child);
  child->parent_ = this;
}
}  // namespace expr
}  // namespace amdb