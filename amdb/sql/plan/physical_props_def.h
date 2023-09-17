#pragma once

#include "sql/plan/rel_opt_node.h"
#include "sql/plan/rel_opt_planner.h"

namespace amdb {
namespace plan {
class PhysicalPropsDef {
 public:
  // virtual RelOptNode* Convert(RelOptPlanner* planner, RelOptNode* node, PhysicalProps* props) = 0;
  // virtual RelOptNode* CanConvert(RelOptPlanner* planner, PhysicalProps* from, PhysicalProps* to) = 0;
  virtual const std::string GetSimpleName() = 0;
};

class ConventionDef : public PhysicalPropsDef {
 public:
  const std::string GetSimpleName() override { return "convention"; }
  static ConventionDef* INSTANCE;

 private:
  ConventionDef(){};
};

class CollationDef : public PhysicalPropsDef {
 public:
  static CollationDef* INSTANCE;

  const std::string GetSimpleName() override { return "collation"; }

 private:
  CollationDef(){};
};

class DistributionDef : public PhysicalPropsDef {
 public:
  static DistributionDef* INSTANCE;
  const std::string GetSimpleName() override { return "distribution"; }

 private:
  DistributionDef(){};
};
}  // namespace plan
}  // namespace amdb
