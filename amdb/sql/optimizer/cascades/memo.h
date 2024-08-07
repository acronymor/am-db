#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "sql/common/statuscode.h"
#include "sql/plan/cost.h"
#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace opt {
using ExprId = std::uint64_t;
using GroupId = std::uint64_t;
using ReducedGroupId = GroupId;

/// Equivalent to MExpr in Columbia/Cascades.
struct RelMemoNode {
  plan::RelOptNode* node;
  std::vector<GroupId> children;
};

using RelMemoNodeRef = std::shared_ptr<RelMemoNode>;

struct Winner {
  bool impossible;
  ExprId expr_id;
  plan::Cost cost;
};

struct GroupInfo {
  std::optional<Winner> winner{std::nullopt};
};

struct Group {
  std::unordered_set<ExprId> group_exprs;
  GroupInfo info;
};

class Memo {
 public:
  Memo() = default;

  ReducedGroupId NextGroupId() {
    this->group_expr_counter_ += 1;
    return this->group_expr_counter_;
  }

  inline void Show() {
    for (const auto& [group_id, group] : this->groups_) {
      for (const auto& expr_id : group.group_exprs) {
        std::cout << "| expr_id=" << expr_id << ", " << this->expr_id_to_expr_node_[expr_id]->node->GetName();
      }
      std::cout << std::endl;
    }
  }

  std::vector<GroupId> GetAllGroupIds();
  std::vector<ExprId> GetAllExprsInGroup(const GroupId& group_id);
  std::optional<ReducedGroupId> GetReduceGroupId(const GroupId& group_id);
  std::optional<GroupId> GetGroupId(const ExprId& expr_id);
  std::optional<Group> GetGroup(const GroupId& group_id);
  std::optional<GroupInfo> GetGroupInfo(const GroupId& group_id);
  void UpdateGroupInfo(const GroupId& group_id, const GroupInfo& group_info);

  plan::RelOptNode* GetBestPlan(const GroupId& group_id);

  ExprId NextExprId() {
    this->group_expr_counter_ += 1;
    return this->group_expr_counter_;
  }

  RelMemoNodeRef GetExprMemoNode(const ExprId& expr_id);
  std::pair<GroupId, ExprId> InitMemo(plan::RelOptNode* node);
  std::pair<GroupId, ExprId> AddNewGroupExpr(plan::RelOptNode* node, const std::optional<GroupId>& group_id);

 private:
  Status getBestGroupBind(plan::RelOptNode* root, const GroupId& group_id);

 private:
  void addExprToGroup(const ExprId& expr_id, const ReducedGroupId& group_id, const RelMemoNodeRef& memo_node);
  ReducedGroupId mergeGroup(const ReducedGroupId& group_a, const ReducedGroupId& group_b);

 private:
  std::unordered_map<ExprId, GroupId> expr_id_to_group_id_{};
  std::unordered_map<ExprId, RelMemoNodeRef> expr_id_to_expr_node_{};
  std::unordered_map<RelMemoNodeRef, ExprId> expr_node_to_expr_id_{};
  std::unordered_map<ReducedGroupId, Group> groups_{};
  std::unordered_map<GroupId, GroupId> merged_groups_{};
  std::uint64_t group_expr_counter_{0};
};

}  // namespace opt
}  // namespace amdb
