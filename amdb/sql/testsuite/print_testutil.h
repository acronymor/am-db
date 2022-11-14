#include <iostream>

#include "sql/chunk/chunk.h"

namespace amdb {
namespace testsuite {
void PrintHexStr(const std::string* str);

void PrintChunk(const chunk::Chunk* chunk);
}  // namespace testsuite
}  // namespace amdb