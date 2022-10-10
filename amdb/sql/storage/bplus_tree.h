#include <iostream>

#include "proto/bptree.pb.h"
#include "sql/storage/bplus_tree_node.h"

namespace amdb {
namespace storage {
class Bptree {
 public:
  explicit Bptree(BptNonLeafNodeProto* node);

  ~Bptree() {
    delete root_;
    root_ = nullptr;
  };

  Status Insert(TreeCtx* ctx, std::string&& key, std::string&& value);

  Status Delete(TreeCtx* ctx, const std::string& key);

  Status GetItem(TreeCtx* ctx, const std::string& key,
                 std::string* output) const;

 private:
  BptNode* root_ = nullptr;
};
}  // namespace storage
}  // namespace amdb