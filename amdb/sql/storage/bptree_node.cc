#include "sql/storage/bptree_node.h"

namespace amdb {
namespace storage {

uint64_t BptreeNode::ID() const {
  return id_;
}

bool BptreeNode::IsLeaf() const {
  return is_leaf_;
}

bool BptreeNode::IsRoot() const {
  return is_root_;
}

void BptreeNode::Serialize(std::string* output) {
  proto_.ParseFromString(*output);
}

void BptreeNode::Deserialize(std::string& input) {
  input = proto_.SerializeAsString();
}
}
}