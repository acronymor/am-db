#include "sql/optimizer/costs/opt_cost.h"

namespace amdb {
namespace opt {
plan::Cost OptCostModel::Zero() {
  return plan::Cost{.cpu_cost = 0.0, .mem_cost = 0.0, .network_cost = 0.0, .disk_cost = 0.0};
}

plan::Cost OptCostModel::Accumulate(plan::Cost& total, plan::Cost& cost) {
  total.cpu_cost += cost.cpu_cost;
  total.disk_cost += cost.disk_cost;
  total.network_cost += cost.network_cost;
  total.mem_cost += cost.mem_cost;
  return total;
}
}  // namespace opt
}  // namespace amdb
