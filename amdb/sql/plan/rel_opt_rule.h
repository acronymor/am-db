#include "sql/context/statement_context.h"
#include "sql/plan/physical_props_def.h"

namespace amdb {
namespace plan {
class RelOptRule {
 public:
  explicit RelOptRule(StatementContext* stmt_ctx);

 protected:
  StatementContext* stmt_ctx;
};

class ConverterRule : public RelOptRule {
 public:
  ConverterRule(StatementContext* stmt_ctx) : RelOptRule(stmt_ctx){};
  virtual RelOptNode* convert(RelOptNode* node) = 0;

 private:
  PhysicalPropsDef* in_prop_;
  PhysicalPropsDef* out_props_;
};

class PhysicalTableScanRule : public ConverterRule {
 public:
  PhysicalTableScanRule(StatementContext* stmt_ctx) : ConverterRule(stmt_ctx){};

  RelOptNode* convert(RelOptNode* node) override;
};
}  // namespace plan
}  // namespace amdb
