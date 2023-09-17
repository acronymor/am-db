#pragma once

namespace amdb {
namespace plan {
class RelOptCost {
 public:
  RelOptCost(double cpu, double io, double rows) : cpu_(cpu), io_(io), rows_(rows) {}

  double GetRows() { return rows_; }

  double GetCpu() { return cpu_; };

  double GetIo() { return io_; };

  bool operator==(const RelOptCost& rhs) const { return cpu_ == rhs.cpu_ && io_ == rhs.io_ && rows_ == rhs.rows_; }
  bool operator!=(const RelOptCost& rhs) const { return !(rhs == *this); }
  bool operator<(const RelOptCost& rhs) const { return cpu_ < rhs.cpu_ && io_ < rhs.io_ && rows_ < rhs.rows_; }
  bool operator>(const RelOptCost& rhs) const { return cpu_ > rhs.cpu_ && io_ > rhs.io_ && rows_ > rhs.rows_; }
  bool operator<=(const RelOptCost& rhs) const { return cpu_ <= rhs.cpu_ && io_ <= rhs.io_ && rows_ <= rhs.rows_; }
  bool operator>=(const RelOptCost& rhs) const { return cpu_ >= rhs.cpu_ && io_ >= rhs.io_ && rows_ >= rhs.rows_; }

 private:
  double cpu_;
  double io_;
  double rows_;
};

struct RelOptCostFactory {
  RelOptCost makeCost(double rows, double cpu, double io) { return RelOptCost(rows, cpu, io); }
};
};  // namespace plan
}  // namespace amdb
