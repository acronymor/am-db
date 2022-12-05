#include "sql/analyzer/analyzer.h"
#include "sql/parser/dml.h"

namespace amdb {
namespace analyzer {
class SelectAnalyzer : public AstAnalyzer {
 public:
  ~SelectAnalyzer() override = default;

  SelectAnalyzer(StatementContext* stmt_ctx) : AstAnalyzer(stmt_ctx) {
    stmt_ = dynamic_cast<parser::SelectStmt*>(stmt_ctx->stmt_ast);
  }

  Status Analyze() override;

 protected:
  parser::SelectStmt* stmt_ = nullptr;
};
}  // namespace analyzer
}  // namespace amdb
