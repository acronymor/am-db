#pragma once

#include "sql/plan/range.h"
#include "sql/plan/rel_opt_node.h"
#include "sql/plan/rel_opt_table.h"

namespace amdb {
namespace plan {
class TableScan : public RelOptNode {
 public:
  RelOptTable* GetTable() const;
  void SetTable(RelOptTable* table);
  uint64_t GetId() override;
  std::vector<IndexRange*>* PrimaryRanges();
  std::vector<expr::ExprNode*>* PrimaryFilters();
  std::vector<expr::ExprNode*>* TableFilters();

 private:
  RelOptTable* table_;
  std::vector<IndexRange*> primary_ranges;
  std::vector<expr::ExprNode*> primary_filters;
  std::vector<expr::ExprNode*> table_filters;
};

class LogicalTableScan : public TableScan {
 public:
  LogicalTableScan() = default;

  uint64_t GetId() override;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  RelOptNodeType GetType() override;

  plan::Cost FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) override;
};

class PhysicalTableScan : public TableScan {
 public:
  PhysicalTableScan() = default;
  const std::string GetName() override;
  uint64_t GetArity() override;
  uint64_t Hash() override;
  uint64_t GetId() override;
  RelOptNodeType GetType() override;

  plan::Cost FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) override;
};
}  // namespace plan
}  // namespace amdb
