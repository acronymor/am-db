#include "sql/optimizer/costs/adaptive_cost.h"

namespace amdb {
namespace opt {
plan::Cost AdaptiveCost::Zero() { return opt_cost_model_.Zero(); }

plan::Cost AdaptiveCost::Accumulate(plan::Cost& total, plan::Cost& cost) {
  return opt_cost_model_.Accumulate(total, cost);
}
}  // namespace opt
}  // namespace amdb
