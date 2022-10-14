#include "sql/chunk/chunk.h"

namespace amdb {
namespace chunk {

// convert index kv to chunk
Status Chunk::PullIndexData(
    std::vector<std::pair<std::string, std::string>>& data_segment) {
  return C_OK;
}

// convert data kv to chunk
Status Chunk::PullKvData(
    std::vector<std::pair<std::string, std::string>>& data_segment) {
  return Status::C_OK;
}

}  // namespace chunk
}  // namespace amdb