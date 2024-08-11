#include <cstdint>
#include <iostream>

#include "sql/optimizer/cascades/memo.h"

namespace amdb {
namespace opt {
class PlaceHolder : public plan::RelOptNode {
 public:
  PlaceHolder(plan::RelOptNode* node, const GroupId& group_id) : node_(node), group_id_(group_id){};
  const std::string GetName() override { return "PlaceHolder"; };
  plan::RelOptNode* GetNode() const {return this->node_;};

  GroupId GetGroupID() const { return this->group_id_; }

  uint64_t GetArity() override { return 0; };
  plan::Cost FindLocalCost(chunk::ColumnDescriptor* desc, const std::vector<plan::Cost>& input_cost) override {
    return plan::Cost{.cpu_cost = 0, .mem_cost = 0, .network_cost = 0, .disk_cost = 0};
  };
  uint64_t Hash() override { return 0; };
  plan::RelOptNodeType GetType() override {
    if (this->node_ == nullptr) {
      return plan::RelOptNodeType::kPlaceHolder;
    }
    return this->node_->GetType();
  };

 private:
  plan::RelOptNode* node_;
  GroupId group_id_;
};
};  // namespace opt
}  // namespace amdb