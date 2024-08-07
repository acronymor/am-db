#include "sql/optimizer/cascades/memo.h"

#include <common/assert.h>

#include "common/log.h"
#include "placeholder.h"

namespace amdb {
namespace opt {

std::vector<GroupId> Memo::GetAllGroupIds() {
  std::vector<GroupId> v;
  v.reserve(this->groups_.size());
  for (const auto& [key, value] : this->groups_) {
    v.push_back(key);
  }
  std::ranges::sort(v);
  return v;
}

std::vector<ExprId> Memo::GetAllExprsInGroup(const GroupId& group_id) {
  const std::optional<ReducedGroupId>& reduced_group_id = this->GetReduceGroupId(group_id);
  if (!reduced_group_id.has_value() || !this->groups_.contains(reduced_group_id.value())) {
    return {};
  }

  std::unordered_set<ExprId>& group_exprs = this->groups_.at(reduced_group_id.value()).group_exprs;
  std::vector<ExprId> v;
  v.reserve(group_exprs.size());
  v.insert(v.end(), group_exprs.begin(), group_exprs.end());
  std::ranges::sort(v);
  return v;
}

std::optional<ReducedGroupId> Memo::GetReduceGroupId(const GroupId& group_id) {
  if (this->merged_groups_.contains(group_id)) {
    return std::make_optional<ReducedGroupId>(this->merged_groups_[group_id]);
  }

  return std::make_optional<ReducedGroupId>(group_id);
}

std::optional<GroupId> Memo::GetGroupId(const ExprId& expr_id) {
  if (!this->expr_id_to_group_id_.contains(expr_id)) {
    return std::nullopt;
  }

  const GroupId& group_id = this->expr_id_to_group_id_[expr_id];
  return this->GetReduceGroupId(group_id);
}

std::optional<Group> Memo::GetGroup(const GroupId& group_id) {
  const std::optional<ReducedGroupId>& reduced_group_id = this->GetReduceGroupId(group_id);
  if (!reduced_group_id.has_value()) {
    return std::nullopt;
  }

  return this->groups_[reduced_group_id.value()];
}

std::optional<GroupInfo> Memo::GetGroupInfo(const GroupId& group_id) {
  const std::optional<ReducedGroupId>& reduced_group_id = this->GetReduceGroupId(group_id);
  if (!reduced_group_id.has_value()) {
    return std::nullopt;
  }

  return std::make_optional(this->groups_[reduced_group_id.value()].info);
}

void Memo::UpdateGroupInfo(const GroupId& group_id, const GroupInfo& group_info) {
  if (group_info.winner) {
    if (!group_info.winner->impossible) {
      ERROR("winner cost")
    }
  }

  const std::optional<ReducedGroupId>& reduced_group_id = this->GetReduceGroupId(group_id);
  if (!reduced_group_id.has_value()) {
    return;
  }

  this->groups_[reduced_group_id.value()].info = group_info;
}

plan::RelOptNode* Memo::GetBestPlan(const GroupId& group_id) {
  PlaceHolder* empty = new PlaceHolder(nullptr, 0);
  Status status = this->getBestGroupBind(empty, group_id);
  AMDB_ASSERT_EQ(Status::C_OK, status);
  plan::RelOptNode* root = empty->GetInput(0);
  return root;
}

Status Memo::getBestGroupBind(plan::RelOptNode* root, const GroupId& group_id) {
  const std::optional<GroupInfo>& group_info = this->GetGroupInfo(group_id);
  const ExprId& expr_id = group_info.value().winner->expr_id;
  const RelMemoNodeRef& node_ref = this->GetExprMemoNode(expr_id);

  node_ref->node->RemoveInputs();
  root->AddInput(node_ref->node);

  for (const auto& item : node_ref->children) {
    this->getBestGroupBind(node_ref->node, item);
  }

  return Status::C_OK;
}

RelMemoNodeRef Memo::GetExprMemoNode(const ExprId& expr_id) { return this->expr_id_to_expr_node_[expr_id]; }

std::pair<GroupId, ExprId> Memo::InitMemo(plan::RelOptNode* node) {
  std::vector<GroupId> children_group_ids;
  for (plan::RelOptNode* input : node->GetInputs()) {
    const auto& [tmp_group_id, _] = this->InitMemo(input);
    children_group_ids.emplace_back(tmp_group_id);
  }

  const ExprId& expr_id = this->NextExprId();
  const GroupId& group_id = this->NextGroupId();

  const RelMemoNodeRef& memo_node_ref = std::make_shared<RelMemoNode>(node, children_group_ids);
  this->expr_id_to_expr_node_.insert(std::make_pair(expr_id, memo_node_ref));
  this->expr_id_to_group_id_.insert(std::make_pair(expr_id, group_id));
  this->expr_node_to_expr_id_.insert(std::make_pair(memo_node_ref, expr_id));
  this->addExprToGroup(expr_id, group_id, memo_node_ref);
  return std::make_pair(group_id, expr_id);
}

std::pair<GroupId, ExprId> Memo::AddNewGroupExpr(plan::RelOptNode* node, const std::optional<GroupId>& group_id) {
  if (!group_id.has_value()) {
    return std::pair<GroupId, ExprId>(0, 0);
  }

  const GroupId real_group_id = group_id.value();
  const std::optional<ReducedGroupId> reduced_group_id = this->GetReduceGroupId(real_group_id);
  if (!reduced_group_id.has_value()) {
    return std::pair<GroupId, ExprId>(0, 0);
  }
  const ReducedGroupId real_reduced_group_id = reduced_group_id.value();

  // this->mergeGroup(group_a, group_id);
  std::vector<GroupId> children_group_ids;
  for (plan::RelOptNode* input : node->GetInputs()) {
    if (input->GetType() == plan::RelOptNodeType::kPlaceHolder) {
      const PlaceHolder* tmp = dynamic_cast<PlaceHolder*>(input);
      const GroupId& tmp_group_id = tmp->GetGroupID();
      children_group_ids.emplace_back(tmp_group_id);
    }
  }

  const ExprId& expr_id = this->NextExprId();
  const RelMemoNodeRef& memo_node_ref = std::make_shared<RelMemoNode>(node, children_group_ids);

  this->expr_id_to_expr_node_.insert(std::make_pair(expr_id, memo_node_ref));
  this->expr_id_to_group_id_.insert(std::make_pair(expr_id, real_reduced_group_id));
  this->expr_node_to_expr_id_.insert(std::make_pair(memo_node_ref, expr_id));
  this->addExprToGroup(expr_id, real_reduced_group_id, memo_node_ref);
  return std::make_pair(real_reduced_group_id, expr_id);
}

void Memo::addExprToGroup(const ExprId& expr_id, const ReducedGroupId& group_id, const RelMemoNodeRef& memo_node) {
  if (this->groups_.contains(group_id)) {
    this->groups_[group_id].group_exprs.insert(expr_id);
    return;
  }

  Group group = {.group_exprs = {expr_id}, .info = {.winner = std::nullopt}};
  this->groups_.insert(std::make_pair(group_id, group));
}

ReducedGroupId Memo::mergeGroup(const ReducedGroupId& group_a, const ReducedGroupId& group_b) {
  // TODO
  if (group_a == group_b) {
    return group_a;
  }

  std::vector<ExprId> group_a_exprs = this->GetAllExprsInGroup(group_a);
  for (auto& expr_id : group_a_exprs) {
    RelMemoNodeRef expr_node = this->expr_id_to_expr_node_[expr_id];
  }

  return group_b;
}

}  // namespace opt
}  // namespace amdb