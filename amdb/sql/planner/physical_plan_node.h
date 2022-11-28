#include "sql/planner/plan_node.h"
#include "sql/planner/range.h"
#include "sql/schema/schema.h"

namespace amdb {
namespace planner {
class PhysicalNode : public PlanNode {
 public:
  enum Type : uint8_t {
    kPhysicalTypeInvalid = 0,
    kPhysicalTableScan = 1,
    kPhysicalFilter = 2,
    kPhysicalResultSet = 3
  };

 public:
  PhysicalNode(Type type) : PlanNode(kPlanPhysical), type(type) {}
  ~PhysicalNode() override = default;

  void AddChild(PhysicalNode* child);
  size_t ChildrenSize() const { return children.size(); }
  PhysicalNode* Child(size_t i) const { return children.at(i); }

 public:
  Type type = kPhysicalTypeInvalid;
  std::vector<PhysicalNode*> children;
};

struct PhysicalTableScan : public PhysicalNode {
  PhysicalTableScan() : PhysicalNode(kPhysicalTableScan) {}
  ~PhysicalTableScan() override = default;

  std::string Name() override { return "PhysicalTableScan"; }
  std::string ToString() override;

  std::vector<IndexRange*> primary_ranges;
  std::vector<expr::ExprNode*> primary_filters;  // known as index_filters
  std::vector<expr::ExprNode*> table_filters;

  schema::TableInfo* table_info{nullptr};
};

struct PhysicalFilter : public PhysicalNode {
  PhysicalFilter() : PhysicalNode(kPhysicalFilter) {}
  ~PhysicalFilter() override = default;

  std::string Name() override { return "PhysicalFilter"; }
  std::string ToString() override;

  std::vector<expr::ExprNode*> conditions;
};

struct PhysicalResultSet : public PhysicalNode {
  PhysicalResultSet() : PhysicalNode(kPhysicalResultSet) {}
  ~PhysicalResultSet() override = default;

  std::string Name() override { return "PhysicalResultSet"; }
  std::string ToString() override;
};
}  // namespace planner
}  // namespace amdb