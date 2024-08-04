#pragma once

namespace amdb {
namespace plan {
struct Cost {
  double cpu_cost;
  double mem_cost;
  double network_cost;
  double disk_cost;
};

inline double EstimateCost(const Cost& cost) {
  return cost.cpu_cost + cost.mem_cost + cost.network_cost + cost.disk_cost;
}

inline bool operator<(const Cost& cost1, const Cost& cost2) { return EstimateCost(cost1) < EstimateCost(cost2); }
inline bool operator<=(const Cost& cost1, const Cost& cost2) { return EstimateCost(cost1) <= EstimateCost(cost2); }
inline bool operator>(const Cost& cost1, const Cost& cost2) { return EstimateCost(cost1) > EstimateCost(cost2); }
inline bool operator>=(const Cost& cost1, const Cost& cost2) { return EstimateCost(cost1) >= EstimateCost(cost2); }
inline bool operator==(const Cost& cost1, const Cost& cost2) { return EstimateCost(cost1) == EstimateCost(cost2); }
inline bool operator!=(const Cost& cost1, const Cost& cost2) { return EstimateCost(cost1) != EstimateCost(cost2); }
}  // namespace plan
}  // namespace amdb