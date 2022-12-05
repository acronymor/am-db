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
  AMDB_ASSERT_EQ(Status::C_OK, parser->error);
  AMDB_ASSERT_EQ(NodeType::NT_SELECT, parser->result[0]->node_type);

  sql = "INSERT INTO t (id, name, gender, score) VALUES (1, 'zhang', 'M', 80)";
  parser->parse(sql);
  AMDB_ASSERT_EQ(Status::C_OK, parser->error);
  AMDB_ASSERT_EQ(NodeType::NT_INSERT, parser->result[1]->node_type);

  sql = "UPDATE t SET name = 'zhang' WHERE name = 'wang' ";
  parser->parse(sql);
  AMDB_ASSERT_EQ(Status::C_OK, parser->error);
  AMDB_ASSERT_EQ(NodeType::NT_UPDATE, parser->result[2]->node_type);
}

TEST_F(ParserTest, GenAstTest) {
  StatementContext stmt_ctx;
  stmt_ctx.raw_sql = "SELECT * FROM t WHERE id = 1";
  parser::GenAst(&stmt_ctx);
  AMDB_ASSERT_EQ(NodeType::NT_SELECT, stmt_ctx.stmt_ast->node_type);

  stmt_ctx.raw_sql = "UPDATE t SET name = 'zhang' WHERE name = 'wang' ";
  parser::GenAst(&stmt_ctx);
  AMDB_ASSERT_EQ(Status::C_OK, parser->error);
  AMDB_ASSERT_EQ(NodeType::NT_UPDATE, stmt_ctx.stmt_ast->node_type);
}
}  // namespace parser
}  // namespace amdb
