#include "sql/analyzer/analyzer.h"
#include "sql/parser/ddl.h"
#include "sql/schema/schema.h"
#include "sql/storage/id_allocator.h"

namespace amdb {
namespace analyzer {
/**
 * Insert database/table
 */
class InsertAnalyzer : public AstAnalyzer {
 public:
  ~InsertAnalyzer() override = default;

  InsertAnalyzer(StatementContext* stmt_ctx) : AstAnalyzer(stmt_ctx) {
    stmt_ = dynamic_cast<parser::InsertStmt*>(stmt_ctx->stmt_ast);
    id_allocator = storage::IdAllocatorSingleton::GetInstance();
  }

  virtual Status Analyze() override;

 protected:
  storage::IdAllocator* id_allocator{nullptr};
  parser::InsertStmt* stmt_{nullptr};
};
}  // namespace analyzer
}  // namespace amdb
