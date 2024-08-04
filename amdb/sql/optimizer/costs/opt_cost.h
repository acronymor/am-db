#include <iostream>
#include "sql/optimizer/costs/cost_model.h"
#include "sql/plan/cost.h"

namespace amdb {
namespace opt {
class OptCostModel : public CostModel {
 public:
  plan::Cost Zero() override;

  plan::Cost Accumulate(plan::Cost& total, plan::Cost& cost) override;
};
}  // namespace opt
}  // namespace amdb