#include "sql/context/statement_context.h"
#include "sql/optimizer/optimizer.h"


namespace amdb {
namespace opt {
class Heuristics final: public Optimizer {
 public:
  Heuristics(StatementContext* stmt_ctx);
  plan::RelOptNode* FindBestExp() override;

 private:
  StatementContext* stmt_ctx_;
};
};
} // amdb

