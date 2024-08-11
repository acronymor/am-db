#include <vector>

#include "sql/optimizer/rules/enumerable_filter_rule.h"
#include "sql/optimizer/rules/enumerable_resultset_rule.h"
#include "sql/optimizer/rules/enumerable_table_scan_rule.h"
#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
static std::vector<RuleWrapper> InitCascadesRules(Arena* arena) {
  std::vector<RuleWrapper> rules;
  rules.emplace_back(arena->CreateObject<EnumerableResultSetRule>(arena), OptimizerType::Cascades);
  rules.emplace_back(arena->CreateObject<EnumerableFilterRule>(arena), OptimizerType::Cascades);
  rules.emplace_back(arena->CreateObject<EnumerableTableScanRule>(arena), OptimizerType::Cascades);
  return rules;
}
};  // namespace opt
};  // namespace amdb