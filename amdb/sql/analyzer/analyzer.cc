#include "sql/analyzer/select_analyzer.h"

namespace amdb {
namespace analyzer {
Status AnalyzeAst(StatementContext* stmt_ctx) {
  std::unique_ptr<AstAnalyzer> ast_analyzer;
  switch (stmt_ctx->stmt_ast->node_type) {
    case parser::NT_SELECT: {
      ast_analyzer = std::make_unique<SelectAnalyzer>(stmt_ctx);
    }; break;
  }

  return ast_analyzer->Analyze();
}
}  // namespace analyzer
}  // namespace amdb
