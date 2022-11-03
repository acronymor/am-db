#include "sql/executor/resultset_executor.h"

namespace amdb {
namespace executor {
Status ResultSetExecutor::Open() { return Status::C_OK; }

Status ResultSetExecutor::Close() { return Status::C_OK; }

Status ResultSetExecutor::Consume(chunk::Chunk *chunk) {
  for (chunk->ResetCursor(); !chunk->ReachEnd(); chunk->Next()) {
    auto row = chunk->CurrentRow();
  }
  return Status::C_OK;
}
}  // namespace executor
}  // namespace amdb
