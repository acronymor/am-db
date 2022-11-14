#include "sql/chunk/chunk.h"

#include "sql/testsuite/print_testutil.h"
#include "sql/testsuite/schema_gen_testutil.h"

namespace amdb {
namespace chunk {
class ChunkTest : public testsuite::SchemaGen {};

TEST_F(ChunkTest, iterator) {
  chunk::Chunk* rows = GenRows(GenRowDesc(), 4);
  testsuite::PrintChunk(rows);
}
}  // namespace chunk
}  // namespace amdb