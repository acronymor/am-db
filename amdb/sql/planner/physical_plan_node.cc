#include "sql/planner/physical_plan_node.h"

namespace amdb {
namespace planner {

std::string PhysicalTableScan::ToString() {
  std::string res;
  res.append("PhysicalTableScan::ToString()");
  return res;
}

std::string PhysicalFilter::ToString() {
  std::string res;
  res.append("PhysicalFilter::ToString()");
  return res;
}

std::string PhysicalResultSet::ToString() {
  std::string res;
  res.append("PhysicalResultSet::ToString()");
  return res;
}
}  // namespace planner
}  // namespace amdb
