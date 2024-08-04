#include "rel_opt_node.h"

#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/plan/filter.h"
#include "sql/plan/result_set.h"

namespace amdb {
namespace plan {
class RelOptNodeTest : public ::testing::Test {
 protected:
  void SetUp() override {}
};

TEST_F(RelOptNodeTest, Basic) {
  RelOptNode* result_set = new PhysicalResultSet();
  RelOptNode* filter = new PhysicalFilter();
  result_set->AddInput(filter);

  AMDB_ASSERT_EQ(filter->GetName(), result_set->GetInput(0)->GetName());
  AMDB_ASSERT_EQ(filter->GetType(), result_set->GetInput(0)->GetType());
  AMDB_ASSERT_EQ(filter, result_set->GetInput(0));
}
}  // namespace plan
}  // namespace amdb
