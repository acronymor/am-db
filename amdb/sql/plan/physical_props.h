#include "sql/plan/physical_props_def.h"

namespace amdb {
namespace plan {
class PhysicalProps {
 public:
  virtual PhysicalPropsDef* GetPhysicalPropsDef() = 0;
};

class ConventionProps : public PhysicalProps {
 public:
  PhysicalPropsDef* GetPhysicalPropsDef() override;
};

class CollationProps : public PhysicalProps {
 public:
  PhysicalPropsDef* GetPhysicalPropsDef() override;
};

class DistributionProps : public PhysicalProps {
 public:
  PhysicalPropsDef* GetPhysicalPropsDef() override;
};
};  // namespace plan

}  // namespace amdb
