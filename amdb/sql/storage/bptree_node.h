#include <iostream>

#include "proto/bptree.pb.h"

namespace amdb {
namespace storage {
class BptreeNode {
 public:
  uint64_t ID() const;
  bool IsLeaf() const;
  bool IsRoot() const;

  void Serialize(std::string* output);

  void Deserialize(std::string& input);

 private:
  uint64_t id_;

  bool is_loaded_ = false;
  bool is_leaf_ = false;
  bool is_root_ = false;

  BpTreeLeafNodeProto proto_;
};
}
}