#pragma once

#include <cstdint>
#include <iostream>

#include "sql/plan/rel_opt_node.h"

namespace amdb {
namespace opt {
enum struct RuleMatcherType : std::uint32_t {
  MatchAndPickNode,
  MatchNode,

  PickOne,
  PickMany,

  IgnoreOne,
  IgnoreMany
};

class RuleMatcher {
 public:
  explicit RuleMatcher(const RuleMatcherType& rule_matcher_type);
  virtual ~RuleMatcher() = default;

  RuleMatcherType GetMatcherType() const;

 private:
  RuleMatcherType rule_matcher_type_;
};

class MatchAndPickNode final : public RuleMatcher {
 public:
  explicit MatchAndPickNode(std::size_t* pick_to, const std::vector<std::shared_ptr<RuleMatcher>>& children, const plan::RelOptNodeType& type);

  std::vector<std::shared_ptr<RuleMatcher>> GetChildren();

  plan::RelOptNodeType GetType() const;

  std::size_t PickTo();

 private:
  std::size_t pick_to_{0};
  plan::RelOptNodeType type_;
  std::vector<std::shared_ptr<RuleMatcher>> children_;
};

class MatchNode final : public RuleMatcher {
 public:
  explicit MatchNode(const std::vector<std::shared_ptr<RuleMatcher>>& children, const plan::RelOptNodeType& type);

 std::vector<std::shared_ptr<RuleMatcher>> GetChildren();

  plan::RelOptNodeType GetType() const;

 private:
  plan::RelOptNodeType type_;
  std::vector<std::shared_ptr<RuleMatcher>> children_;
};

class PickOne final : public RuleMatcher {
 public:
  explicit PickOne(std::size_t* pick_to, const bool& expand);

  bool GetExpand() const;

 private:
  std::size_t pick_to{0};
  bool expand_{false};
};

class PickMany final : public RuleMatcher {
 public:
  explicit PickMany(std::size_t* pick_to);

 private:
  std::size_t pick_to{0};
};

class IgnoreOne final : public RuleMatcher {
 public:
  IgnoreOne();
};

class IgnoreMany final : public RuleMatcher {
 public:
  IgnoreMany();
};

};  // namespace opt
}  // namespace amdb
