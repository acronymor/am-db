#include "sql/parser/parser.h"

#include "common/assert.h"
#include "gtest/gtest.h"

namespace amdb {
namespace parser {
class ParserTest : public testing::Test {
 protected:
  void SetUp() override { parser = new SqlParser(); }

  void TearDown() override { delete parser; }

 protected:
  SqlParser* parser;
};

TEST_F(ParserTest, SQLTest) {
  std::string sql = "SELECT * FROM t";
  parser->parse(sql);
  AMDB_ASSERT_EQ(ParseError::SUCC, parser->error);
  AMDB_ASSERT_EQ(NodeType::NT_SELECT, parser->result[0]->node_type);

  sql = "INSERT INTO t (id, name, gender, score) VALUES (1, 'zhang', 'M', 80)";
  parser->parse(sql);
  AMDB_ASSERT_EQ(ParseError::SUCC, parser->error);
  AMDB_ASSERT_EQ(NodeType::NT_INSERT, parser->result[1]->node_type);

  sql = "UPDATE t SET name = 'zhang' WHERE name = 'wang' ";
  parser->parse(sql);
  AMDB_ASSERT_EQ(ParseError::SUCC, parser->error);
  AMDB_ASSERT_EQ(NodeType::NT_UPDATE, parser->result[2]->node_type);
}
}  // namespace parser
}  // namespace amdb
