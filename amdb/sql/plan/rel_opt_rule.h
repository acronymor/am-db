#pragma once

#include "sql/context/statement_context.h"
#include "sql/plan/create_database.h"
#include "sql/plan/create_table.h"
#include "sql/plan/filter.h"
#include "sql/plan/insert.h"
#include "sql/plan/rel_trait_set.h"
#include "sql/plan/result_set.h"
#include "sql/plan/table_scan.h"

namespace amdb {
namespace plan {

enum struct RelOptRuleType : uint8_t {
  kPhysicalFilterRule = 0,
  kPhysicalTableScanRule = 1,
  kCreateDatabaseRule = 2,
  kCreateTableRule = 3,
  kInsertTableRule = 4,
  kPhysicalResultSetRule = 5,
};

class RelOptRule {
 public:
  RelOptRule(RelOptNode* original, RelOptNode* substitute, std::uint64_t arity)
      : original_(original), substitute_(substitute), arity_(arity){};

  RelOptRule(const RelOptRule& rule);

  // condition
  virtual bool Match(RelOptNode* before, RelOptNode* after) { return true; }

  virtual RelOptNode* Substitute(RelOptNode* before, RelTraitSet* trait_set) = 0;

  virtual RelOptRuleType GetType() = 0;
  virtual uint64_t GetId() = 0;
  virtual std::string GetName() = 0;

  RelOptNode* GetOriginal() const { return original_; }
  // next_substitute
  RelOptNode* GetSubstitute() const { return substitute_; }
  int GetArity() const { return arity_; }

 private:
  RelOptNode* original_;
  RelOptNode* substitute_;
  int arity_;
};

class PhysicalTableScanRule : public RelOptRule {
 public:
  PhysicalTableScanRule();
  RelOptNode* Substitute(RelOptNode* before, RelTraitSet* trait_set) override;

  uint64_t GetId() override;
  std::string GetName() override;
  RelOptRuleType GetType() override;
};

class PhysicalFilterRule : public RelOptRule {
 public:
  PhysicalFilterRule();
  RelOptNode* Substitute(RelOptNode* before, RelTraitSet* trait_set) override;

  uint64_t GetId() override;
  std::string GetName() override;
  RelOptRuleType GetType() override;
};

class PhysicalResultSetRule : public RelOptRule {
 public:
  PhysicalResultSetRule();
  RelOptNode* Substitute(RelOptNode* before, RelTraitSet* trait_set) override;

  uint64_t GetId() override;
  std::string GetName() override;
  RelOptRuleType GetType() override;
};
}  // namespace plan
}  // namespace amdb
