#include "sql/plan/physical_props.h"

namespace amdb {
namespace plan {
PhysicalPropsDef* ConventionProps::GetPhysicalPropsDef() { return ConventionDef::INSTANCE; }
PhysicalPropsDef* CollationProps::GetPhysicalPropsDef() { return CollationDef::INSTANCE; }
PhysicalPropsDef* DistributionProps::GetPhysicalPropsDef() { return DistributionDef::INSTANCE; }
}  // namespace plan
}  // namespace amdb