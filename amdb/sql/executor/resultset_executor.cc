#include "sql/executor/resultset_executor.h"

#include "sql/testsuite/print_testutil.h"

namespace amdb {
namespace executor {
Status ResultSetExecutor::Open() { return Status::C_OK; }

Status ResultSetExecutor::Close() { return Status::C_OK; }

Status ResultSetExecutor::Consume(chunk::Chunk* chunk) {
  for (chunk->ResetCursor(); !chunk->ReachEnd(); chunk->Next()) {
    auto row = chunk->CurrentRow();
    for (chunk::ColumnDescriptor* desc : row->desc->GetAllColumnDesc()) {
      expr::ExprValue value = row->GetColValue(row->desc->ID(), desc->ID());
      std::string str = value.ToString();
      std::cout << " | " << str;
    }
    std::cout << " |" << std::endl;
  }
  return Status::C_OK;
}
}  // namespace executor
}  // namespace amdb
