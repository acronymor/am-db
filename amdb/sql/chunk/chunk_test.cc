#include "sql/chunk/chunk.h"

#include "sql/chunk/chunk_util.h"
#include "sql/testsuite/print_testutil.h"
#include "sql/testsuite/table_testutil.h"

namespace amdb {
namespace chunk {
class ChunkTest : public testsuite::TableDataGen {};

TEST_F(ChunkTest, iterator) {
  chunk::Chunk* rows = table_gen.GenRows(table_->table_info_, 10);
  testsuite::PrintChunk(rows);
}
}  // namespace chunk
}  // namespace amdb