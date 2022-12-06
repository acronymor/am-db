#include "sql/analyzer/analyzer.h"
#include "sql/parser/ddl.h"

namespace amdb {
namespace analyzer {
/**
 * create database/table/index
 */
class CreateAnalyzer : public AstAnalyzer {
 public:
  ~CreateAnalyzer() override = default;

  CreateAnalyzer(StatementContext* stmt_ctx) : AstAnalyzer(stmt_ctx) {
    stmt_ = dynamic_cast<parser::DdlNode*>(stmt_ctx->stmt_ast);
  }

  virtual Status Analyze() override = 0;

 protected:
  parser::DdlNode* stmt_ = nullptr;
};

class CreateDatabaseAnalyzer : public CreateAnalyzer {
 public:
  ~CreateDatabaseAnalyzer() override = default;

  CreateDatabaseAnalyzer(StatementContext* stmt_ctx) : CreateAnalyzer(stmt_ctx) {
    stmt_ = dynamic_cast<parser::CreateDatabaseStmt*>(stmt_ctx->stmt_ast);
  }

  Status Analyze() override;

 protected:
  parser::CreateDatabaseStmt* stmt_ = nullptr;
};

class CreateTableAnalyzer : public CreateAnalyzer {
 public:
  ~CreateTableAnalyzer() override = default;

  CreateTableAnalyzer(StatementContext* stmt_ctx) : CreateAnalyzer(stmt_ctx) {
    stmt_ = dynamic_cast<parser::CreateTableStmt*>(stmt_ctx->stmt_ast);
  }

  Status Analyze() override;

 protected:
  parser::CreateTableStmt* stmt_ = nullptr;
};
}  // namespace analyzer
}  // namespace amdb
