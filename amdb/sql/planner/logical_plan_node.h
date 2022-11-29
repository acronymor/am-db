#include "sql/planner/plan_node.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace planner {
struct LogicalNode : public PlanNode {
 public:
  enum Type : uint8_t {
    kLogicalTypeInvalid = 0,
    kLogicalTableScan = 1,
    kLogicalFilter = 2,
    kLogicalResultSet = 3
  };

 public:
  LogicalNode(Type type) : PlanNode(kPlanLogical), type(type) {}
  ~LogicalNode() override = default;

  void AddChild(LogicalNode* child);
  size_t ChildrenSize() const { return children.size(); }
  LogicalNode* Child(size_t i) const { return children.at(i); }

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
  std::vector<expr::ExprNode*> conditions;
};

class LogicalFilter : public LogicalNode {
 public:
  LogicalFilter() : LogicalNode(kLogicalFilter) {}
  ~LogicalFilter() override = default;

  std::string Name() override { return "LogicalFilter"; }
  std::string ToString() override;

  std::vector<expr::ExprNode*> conditions;
};
}  // namespace planner
}  // namespace amdb