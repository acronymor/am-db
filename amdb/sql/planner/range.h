#include <iostream>

#include "sql/expression/expr_value.h"

namespace amdb {
namespace planner {

struct IndexRange {
  enum RangePointType : uint8_t {
    LEFT_CLOSE = 0,
    LEFT_OPEN = 1,
    RIGHT_OPEN = 2,
    RIGHT_CLOSE = 3,
  };

  struct RangePoint {
    std::vector<expr::ExprValue> value;
    RangePointType type = LEFT_OPEN;
  };

  RangePoint lower, upper;
  std::string lower_str, upper_str;
};

void Union(const IndexRange* r1, const IndexRange* r2, IndexRange* new_range);
void Intersection(const IndexRange* r1, const IndexRange* r2, IndexRange* new_range);

}  // namespace planner
}  // namespace amdb