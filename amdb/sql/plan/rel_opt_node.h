#pragma once

#include <atomic>
#include <cstdint>
#include <iostream>
#include <vector>
#include <type_traits>

#include "sql/chunk/column_description.h"
#include "sql/context/statement_context.h"
#include "sql/plan/cost.h"

namespace amdb {
namespace plan {
enum struct RelOptNodeType : std::uint32_t {
  kPlaceHolder = 0,
  kLogicalTypeInvalid = 1,

  kLogicalTableScan = 100,
  kLogicalFilter = 101,
  kLogicalResultSet = 102,
  kLogicalCreateDatabase = 103,
  kLogicalCreateTable = 104,
  kLogicalInsert = 105,

  kPhysicalTableScan = 200,
  kPhysicalFilter = 201,
  kPhysicalResultSet = 202,
  kPhysicalCreateDatabase = 203,
  kPhysicalCreateTable = 204,
  kPhysicalInsert = 205,

  kRelsubset = 1000
};

inline std::ostream& operator<<(std::ostream& stream, const RelOptNodeType& opt_node_type) {
  stream<< static_cast<std::underlying_type<RelOptNodeType>::type>(opt_node_type);
  return stream;
}

struct RelOptNode {
  virtual uint64_t GetId() { return id_.fetch_add(1); };

  virtual const std::string GetName() = 0;

  void AddInput(RelOptNode* node);

  RelOptNode* GetInput(const uint64_t& i) {
    const std::vector<RelOptNode*>& inputs = GetInputs();
    return inputs.at(i);
  }

  std::vector<RelOptNode*> GetInputs() { return inputs_; }

  bool IsEnforcer() { return false; };

  virtual uint64_t GetArity() = 0;

  virtual plan::Cost FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) = 0;

  virtual uint64_t Hash() = 0;

  virtual RelOptNodeType GetType() = 0;

  virtual bool IsLogical() {
    const std::uint32_t type = static_cast<uint32_t>(GetType());
    return type >= 100 && type < 200;
  }

  virtual bool IsPhysical() {
    const std::uint32_t type = static_cast<uint32_t>(GetType());
    return type >= 200 && type < 300;
  }

 private:
  std::atomic<uint64_t> id_{0};
  std::vector<RelOptNode*> inputs_;
};
}  // namespace plan
}  // namespace amdb