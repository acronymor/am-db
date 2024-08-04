#pragma once

#include <format>
#include <string>
#include <vector>

#include "sql/plan/cost.h"

namespace amdb {
namespace opt {

class CostModel {
 public:
  virtual ~CostModel() = default;

  virtual plan::Cost Zero() = 0;

  virtual plan::Cost Accumulate(plan::Cost& total, plan::Cost& cost) = 0;

  virtual inline std::string Explain(plan::Cost& cost) {
    std::string res = std::format("cpu_cost={}, mem_cost={}, netword_cost={}, disk_cost={}", cost.cpu_cost,
                                  cost.mem_cost, cost.network_cost, cost.disk_cost);
    return res;
  }

  inline plan::Cost Sum(const plan::Cost& cost, std::vector<plan::Cost>& inputs) {
    plan::Cost total = cost;

    for (plan::Cost& item : inputs) {
      total = this->Accumulate(total, item);
    }

    return total;
  }
};
}  // namespace opt
}  // namespace amdb