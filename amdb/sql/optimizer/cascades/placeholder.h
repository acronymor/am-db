#include <cstdint>
#include <iostream>

namespace amdb {
namespace opt {
class PlaceHolder : public plan::RelOptNode {
 public:
  PlaceHolder(const plan::RelOptNodeType &type, const GroupId& group_id) : type_(type), group_id_(group_id){};
  const std::string GetName() override { return "PlaceHolder"; };

  GroupId GetGroupID() const { return this->group_id_; }

  uint64_t GetArity() override { return 0; };
  plan::Cost FindLocalCost(chunk::ColumnDescriptor *desc, const std::vector<plan::Cost> &input_cost) override {
    return plan::Cost{.cpu_cost = 0, .mem_cost = 0, .network_cost = 0, .disk_cost = 0};
  };
  uint64_t Hash() override { return 0; };
  plan::RelOptNodeType GetType() override { return this->type_; };

 private:
  plan::RelOptNodeType type_;
  GroupId group_id_;
};
};  // namespace opt
}  // namespace amdb