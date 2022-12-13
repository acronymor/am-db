#pragma once

#include "sql/chunk/row_description.h"
#include "sql/planner/plan_node.h"
#include "sql/planner/range.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace planner {
struct LogicalNode : public PlanNode {
 public:
  enum Type : uint8_t {
    kLogicalTypeInvalid = 0,
    kLogicalTableScan = 1,
    kLogicalFilter = 2,
    kLogicalResultSet = 3,
    kLogicalCreateDatabase = 4,
    kLogicalCreateTable = 5,
    kLogicalInsert = 6
  };

 public:
  LogicalNode(Type type) : PlanNode(kPlanLogical), type(type) {}
  ~LogicalNode() override = default;

  void AddChild(LogicalNode* child);
  size_t ChildrenSize() const { return children.size(); }
  LogicalNode* Child(size_t i) const { return children.at(i); }

  virtual void Accept(PlanNodeVisitor* visitor);

  void PrintPlan(std::vector<std::string>& lines, const std::string& ident);

 public:
  std::vector<LogicalNode*> children;
  Type type = kLogicalTypeInvalid;
};

class LogicalTableScan : public LogicalNode {
 public:
  LogicalTableScan() : LogicalNode(kLogicalTableScan) {}
  ~LogicalTableScan() override = default;

  std::string Name() override { return "LogicalTableScan"; }
  std::string ToString() override;

  schema::TableInfo* table_info{nullptr};
  std::vector<IndexRange*> primary_ranges;
  std::vector<expr::ExprNode*> primary_filters;  // known as index_filters
  std::vector<expr::ExprNode*> table_filters;
};

class LogicalFilter : public LogicalNode {
 public:
  LogicalFilter() : LogicalNode(kLogicalFilter) {}
  ~LogicalFilter() override = default;

  std::string Name() override { return "LogicalFilter"; }
  std::string ToString() override;

  std::vector<expr::ExprNode*> conditions;
};

class LogicalResultSet : public LogicalNode {
 public:
  LogicalResultSet() : LogicalNode(kLogicalResultSet) {}
  ~LogicalResultSet() override = default;

  std::string Name() override { return "LogicalResultSet"; }
  std::string ToString() override;
};

class LogicalCreateDatabase : public LogicalNode {
 public:
  LogicalCreateDatabase() : LogicalNode(kLogicalCreateDatabase) {}
  ~LogicalCreateDatabase() override = default;

  std::string Name() override { return "LogicalCreateDatabase"; }
  std::string ToString() override;

  schema::DatabaseInfo* database_info;
};

class LogicalCreateTable : public LogicalNode {
 public:
  LogicalCreateTable() : LogicalNode(kLogicalCreateTable) {}
  ~LogicalCreateTable() override = default;

  std::string Name() override { return "LogicalCreateTable"; }
  std::string ToString() override;

  schema::TableInfo* table_info;
};

class LogicalInsert : public LogicalNode {
 public:
  LogicalInsert() : LogicalNode(kLogicalInsert) {}
  ~LogicalInsert() override = default;

  std::string Name() override { return "LogicalInsert"; }
  std::string ToString() override;

  schema::TableInfo* table_info;
  std::vector<schema::ColumnInfo*> columns;
  std::vector<std::vector<expr::ExprNode*>> expr_nodes;
  chunk::RowDescriptor* row_desc{nullptr};
};
}  // namespace planner
}  // namespace amdb