#pragma once

#include "sql/plan/rel_opt_cost.h"

namespace amdb {
namespace plan {
class RelOptPlanner {
  RelOptCostFactory* GetCostFactory() {
    RelOptCostFactory* factory = new RelOptCostFactory();
    return factory;
  }
};
}  // namespace plan
}  // namespace amdb
