#include "sql/codec/rccodec.h"

namespace amdb {
namespace codec {

size_t EncodeColumn(const chunk::Row *row, std::string *key, std::string *value) {
  return 0;
}

size_t DecodeColumn(const std::string &key, const std::string &value,
                    chunk::Row *row) {
  return 0;
}

}  // namespace codec
}  // namespace amdb
