#include "sql/optimizer/rules/rule.h"

namespace amdb {
namespace opt {

std::string Rule::Name() const {
  return this->name_;
}
}  // namespace opt
}
