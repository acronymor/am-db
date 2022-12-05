#include "sql/common/statuscode.h"
#include "sql/context/statement_context.h"

namespace amdb {
namespace analyzer {
Status AnalyzeAst(StatementContext* stmt_ctx);

class AstAnalyzer {
 public:
  AstAnalyzer(StatementContext* stmt_ctx) : stmt_ctx_(stmt_ctx) {}
  virtual ~AstAnalyzer() = default;

  virtual Status Analyze() = 0;

 protected:
  StatementContext* stmt_ctx_ = nullptr;
};
}  // namespace analyzer
}  // namespace amdb