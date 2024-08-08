#pragma once

#include <unordered_map>

#include "sql/context/statement_context.h"
#include "sql/optimizer/rules/rule_matcher.h"

namespace amdb {
namespace opt {
enum struct OptimizerType : std::uint32_t;
class Optimizer;
using RuleId = std::uint32_t;

class Rule {
 public:
  explicit Rule(const std::string& name) : name_(name) {}
  virtual ~Rule() = default;

  virtual std::shared_ptr<RuleMatcher> Matcher() = 0;
  virtual plan::RelOptNode* Apply(const Optimizer* optimizer, plan::RelOptNode* node) = 0;
  std::string Name() const;

 private:
  std::string name_;
};

struct RuleWrapper {
  const std::shared_ptr<Rule> rule;
  const OptimizerType optimizer_type;
};

}  // namespace opt
}  // namespace amdb