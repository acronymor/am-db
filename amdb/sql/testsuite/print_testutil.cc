#include "sql/testsuite/print_testutil.h"

#include "sql/chunk/column_description.h"
#include "sql/chunk/iterator.h"

namespace amdb {
namespace testsuite {
void PrintHexStr(const std::string* str) {
  for (size_t i = 0; i < str->length(); i++) {
    printf("0x%08hhX ", str->at(i));
  }
  printf("\n");
}

void PrintChunk(const chunk::Chunk* chunk) {
  chunk::Iterator itr(chunk);
  for (itr.Open(); itr.HasNext(); itr.Next()) {
    chunk::Row* row = itr.GetRow();
    std::string str_build;
    for (chunk::ColumnDescriptor* desc : row->desc->GetAllColumnDesc()) {
      expr::ExprValue value = row->GetColValue(row->desc->ID(), desc->ID());
      str_build.append(value.ToString());
      str_build.append("\t");
    }
    PrintHexStr(&str_build);
  }
}
}  // namespace testsuite
}  // namespace amdb