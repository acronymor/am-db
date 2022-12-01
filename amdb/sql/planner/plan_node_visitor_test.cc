#include "sql/planner/plan_node_visitor.h"

#include "gtest/gtest.h"

namespace amdb {
namespace planner {
class PlanNodeVisitorTest : public ::testing::Test {};

TEST_F(PlanNodeVisitorTest, TestLogicalVisitor) {
  LogicalResultSet result_set;
  LogicalFilter filter;
  LogicalTableScan table_scan;

  result_set.AddChild(&filter);
  filter.AddChild(&table_scan);

  LogicalVisitor visitor;
  visitor.Visit(&result_set);
}

TEST_F(PlanNodeVisitorTest, TestPhysicalVisitor) {
  PhysicalResultSet result_set;
  PhysicalFilter filter;
  PhysicalTableScan table_scan;

  result_set.AddChild(&filter);
  filter.AddChild(&table_scan);

  PhysicalVisitor visitor;
  visitor.Visit(&result_set);
}
}  // namespace planner
}  // namespace amdb