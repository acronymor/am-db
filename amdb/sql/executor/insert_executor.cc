#include "sql/executor/insert_executor.h"

#include "sql/expression/literal_expr.h"

namespace amdb {
namespace executor {
Status InsertExecutor::Open() {
  return Status::C_OK;
}

Status InsertExecutor::Close() { return Status::C_OK; }

Status InsertExecutor::Consume(chunk::Chunk* chunk) {
  if(chunk->GetRowDesc() == nullptr) {
    chunk->SetRowDesc(row_desc_);
  }

  for (size_t i = 0; i < insert_plan_->ExprNodes().size(); i++) {
    chunk::Row* row = ctx_->arena->CreateObject<chunk::Row>(ctx_->arena, row_desc_);
    chunk->AddRow(row);

    std::vector<expr::ExprNode*>& record = insert_plan_->ExprNodes().at(i);
    for (size_t j = 0; j < record.size(); j++) {
      expr::LiteralExpr* cell = dynamic_cast<expr::LiteralExpr*>(record.at(j));
      row->SetColValue(row_desc_->ID(), row_desc_->GetColumnDesc(j)->ID(), cell->GetValue());
    }
  }

  Status status = table_->Insert(chunk);
  if (status != Status::C_OK) {
    ERROR("insert failed, {}", status);
  }
  return Status::C_OK;
}
}  // namespace executor
}  // namespace amdb
