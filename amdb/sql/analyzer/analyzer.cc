#include "common/log.h"
#include "sql/analyzer/create_analyzer.h"
#include "sql/analyzer/select_analyzer.h"

namespace amdb {
namespace analyzer {
Status AnalyzeAst(StatementContext* stmt_ctx) {
  std::unique_ptr<AstAnalyzer> ast_analyzer;
  switch (stmt_ctx->stmt_ast->node_type) {
    case parser::NT_SELECT:
      ast_analyzer = std::make_unique<SelectAnalyzer>(stmt_ctx);
      break;
    case parser::NT_CREATE_DATABASE:
      ast_analyzer = std::make_unique<CreateDatabaseAnalyzer>(stmt_ctx);
      break;
    case parser::NT_CREATE_TABLE:
      ast_analyzer = std::make_unique<CreateTableAnalyzer>(stmt_ctx);
      break;
    default:
      ERROR("Not support analyze type {}", stmt_ctx->stmt_ast->node_type);
      break;
  }

  return ast_analyzer->Analyze();
}
}  // namespace analyzer
}  // namespace amdb
