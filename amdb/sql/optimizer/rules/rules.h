#include <vector>

#include "sql/optimizer/rules/enumerable_filter_rule.h"
#include "sql/optimizer/rules/enumerable_resultset_rule.h"
#include "sql/optimizer/rules/enumerable_table_scan_rule.h"
#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {
Status static InitCascadesRules(std::vector<RuleWrapper>* rules) {
  rules->emplace_back(std::make_shared<EnumerableResultSetRule>(), OptimizerType::Cascades);
  rules->emplace_back(std::make_shared<EnumerableFilterRule>(), OptimizerType::Cascades);
  rules->emplace_back(std::make_shared<EnumerableTableScanRule>(), OptimizerType::Cascades);

  return Status::C_OK;
}
};  // namespace opt
};  // namespace amdb