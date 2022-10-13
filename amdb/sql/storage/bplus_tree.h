#include <iostream>

#include "common/assert.h"
#include "proto/bptree.pb.h"
#include "sql/storage/bplus_tree_node.h"

namespace amdb {
namespace storage {
class Bptree {
 public:
  explicit Bptree(TreeCtx* tree_ctx, BptNonLeafNodeProto* node);

  ~Bptree() { root_ = nullptr; };

  Status Insert(std::string&& key, std::string&& value);

  Status Delete(const std::string& key);

  Status GetItem(const std::string& key, std::string* output) const;

  BptNode* Root();

 private:
  BptNode* root_{nullptr};
  TreeCtx* tree_ctx_{nullptr};
};
}  // namespace storage
}  // namespace amdb