
#include "sql/optimizer/costs/opt_cost.h"
#include "sql/optimizer/costs/cost_model.h"
#include "sql/plan/cost.h"
namespace amdb {
namespace opt {

class AdaptiveCost : public CostModel {
 public:
  plan::Cost Zero() override;

  plan::Cost Accumulate(plan::Cost &total, plan::Cost &cost) override;

private:
  OptCostModel opt_cost_model_;
};

}  // namespace opt
}  // namespace amdb
