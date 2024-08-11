#pragma once

#include "sql/context/statement_context.h"
#include "sql/optimizer/rules/rule_matcher.h"

namespace amdb {
namespace opt {
enum struct OptimizerType : std::uint32_t;
class Optimizer;
using RuleId = std::uint32_t;

class Rule {
 public:
  explicit Rule(Arena* arena, const std::string& name) : name_(name), arena_(arena) {}
  virtual ~Rule() = default;

  virtual RuleMatcher* Matcher() = 0;
  virtual plan::RelOptNode* Apply(plan::RelOptNode* node) = 0;
  std::string Name() const;

 protected:
  Arena* arena_;

 private:
  std::string name_;
};

struct RuleWrapper {
  Rule* rule;
  OptimizerType optimizer_type;
};

}  // namespace opt
}  // namespace amdb