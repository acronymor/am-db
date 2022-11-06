#include "sql/chunk/chunk.h"

#include "sql/chunk/iterator.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace chunk {
namespace {
void __printf__(const std::string& value) {
  printf("value=");
  for (size_t i = 0; i < value.length(); i++) {
    printf("0x%02hhX ", value.at(i));
  }
  printf("\n");
}
}  // namespace

class ChunkTest : public testsuite::SchemaGen {};

TEST_F(ChunkTest, iterator) {
  chunk::Chunk* rows = GenRows(GenRowDesc(), 4);
  chunk::Iterator itr(rows);
  for (itr.Open(); itr.HasNext(); itr.Next()) {
    chunk::Row* row = itr.GetRow();
    for (ColumnDescriptor* desc : row->desc->GetAllColumnDesc()) {
      expr::ExprValue value = row->GetColValue(0, desc->ID());
      __printf__(value.ToString());
    }
    std::cout << "-----------------" << std::endl;
  }
}
}  // namespace chunk
}  // namespace amdb