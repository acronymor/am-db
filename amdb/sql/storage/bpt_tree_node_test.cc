#include "common/assert.h"
#include "gtest/gtest.h"
#include "sql/codec/schema_codec.h"
#include "sql/storage/bplus_tree_node.h"
#include "sql/testsuite/kv_data_testutil.h"

namespace amdb {
namespace storage {
class BplusTreeNodeTest : public testing::Test {
 protected:
  void SetUp() override { tree_ctx_ = new TreeCtx(data_test.GetKvApi(), data_test.GetArena(), schema_); }

  void TearDown() override {
    delete tree_ctx_;
    data_test.ClearAll();
  }

 protected:
  TreeCtx* tree_ctx_;
  TreeCtx::Schema schema_ = {0, 0, 0, 0, 0};

  testsuite::KvDataTest data_test;
};

TEST_F(BplusTreeNodeTest, Serialize) {
  BptNodeProto root_proto1;
  root_proto1.set_id(schema_.root_id);
  BptNode root1(tree_ctx_, &root_proto1);
  BptNode leaf1(tree_ctx_, &root1, true);

  leaf1.kvs_.emplace("key-a", "value-a");
  leaf1.kvs_.emplace("key-b", "value-b");
  leaf1.kvs_.emplace("key-c", "value-c");

  std::string str;
  Status status1 = leaf1.Serialize(&str);
  AMDB_ASSERT_EQ(Status::C_OK, status1);

  BptNodeProto root_proto2;
  root_proto2.set_id(schema_.root_id);
  BptNode root2(tree_ctx_, &root_proto2);
  BptNode leaf2(tree_ctx_, &root2, true);
  Status status2 = leaf2.Deserialize(str);
  AMDB_ASSERT_EQ(Status::C_OK, status2);

  AMDB_ASSERT_EQ(leaf1.kvs_.at("key-a"), leaf2.kvs_.at("key-a"));
  AMDB_ASSERT_EQ(leaf1.kvs_.at("key-b"), leaf2.kvs_.at("key-b"));
  AMDB_ASSERT_EQ(leaf1.kvs_.at("key-c"), leaf2.kvs_.at("key-c"));
};
};  // namespace storage
};  // namespace amdb
