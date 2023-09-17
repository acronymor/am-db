#include "sql/plan/physical_props_def.h"

namespace amdb {
namespace plan {
ConventionDef* ConventionDef::INSTANCE = new ConventionDef();
CollationDef* CollationDef::INSTANCE = new CollationDef();
DistributionDef* DistributionDef::INSTANCE = new DistributionDef();
}  // namespace plan
}  // namespace amdb
