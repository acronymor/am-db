#include "sql/optimizer/costs/opt_cost.h"

#include <common/assert.h>

#include "gtest/gtest.h"

namespace amdb {
namespace opt {
struct OptCostTest : public ::testing::Test {};

TEST_F(OptCostTest, CostTest) {
  OptCostModel cost_model;

  plan::Cost zero = cost_model.Zero();
  AMDB_ASSERT_EQ(0, zero.cpu_cost);
  AMDB_ASSERT_EQ(0, zero.mem_cost);
  AMDB_ASSERT_EQ(0, zero.network_cost);
  AMDB_ASSERT_EQ(0, zero.disk_cost);

  plan::Cost total = {1.0, 2.0, 3.0, 4.0};
  plan::Cost cost = {1.0, 2.0, 3.0, 4.0};
  plan::Cost res = cost_model.Accumulate(total, cost);

  AMDB_ASSERT_EQ(2.0, res.cpu_cost);
  AMDB_ASSERT_EQ(4.0, res.mem_cost);
}
};  // namespace opt
};  // namespace amdb