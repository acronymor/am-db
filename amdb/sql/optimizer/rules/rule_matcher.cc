
#include "sql/optimizer/rules/rule_matcher.h"

namespace amdb {
namespace opt {
RuleMatcher::RuleMatcher(const RuleMatcherType& rule_matcher_type) : rule_matcher_type_(rule_matcher_type) {}

RuleMatcherType RuleMatcher::GetMatcherType() const { return this->rule_matcher_type_; }

MatchAndPickNode::MatchAndPickNode(std::size_t* pick_to, const std::vector<RuleMatcher*>& children,
                                   const plan::RelOptNodeType& type)
    : RuleMatcher(RuleMatcherType::MatchAndPickNode), children_(children), type_(type) {
  this->pick_to_ = *pick_to;
  *pick_to += 1;
}

const std::vector<RuleMatcher*>& MatchAndPickNode::GetChildren() { return this->children_; };

plan::RelOptNodeType MatchAndPickNode::GetType() const { return this->type_; }

std::size_t MatchAndPickNode::PickTo() { return this->pick_to_; }

MatchNode::MatchNode(const std::vector<RuleMatcher*>& children, const plan::RelOptNodeType& type)
    : RuleMatcher(RuleMatcherType::MatchNode), children_(children), type_(type) {}

const std::vector<RuleMatcher*>& MatchNode::GetChildren() { return this->children_; }

plan::RelOptNodeType MatchNode::GetType() const { return this->type_; }

PickOne::PickOne(std::size_t* pick_to, const bool expand) : RuleMatcher(RuleMatcherType::PickOne), expand_(expand) {
  this->pick_to = *pick_to;
  *pick_to += 1;
}

bool PickOne::GetExpand() const { return this->expand_; }

PickMany::PickMany(std::size_t* pick_to) : RuleMatcher(RuleMatcherType::PickMany) {
  this->pick_to = *pick_to;
  *pick_to += 1;
}

IgnoreOne::IgnoreOne() : RuleMatcher(RuleMatcherType::IgnoreOne) {}

IgnoreMany::IgnoreMany() : RuleMatcher(RuleMatcherType::IgnoreMany) {}

}  // namespace opt
}  // namespace amdb