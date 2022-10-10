#include "sql/codec/codec.h"

namespace amdb {
namespace codec {

size_t EncodeBytes(const std::string& in, std::string* out) {
  uint32_t len = in.size();
  size_t ret = EncodeCmpUInt32(len, out);
  out->append(in.data(), in.size());
  return in.size() + ret;
}

size_t DecodeBytes(const std::string& in, std::string* out) {
  uint32_t len = 0;
  size_t ret = DecodeCmpUInt32(in, &len);
  if (ret + len <= in.size()) {
    out->append(in.data() + ret, len);
    return len + ret;
  } else {
    return -1;
  }
}

size_t EncodeCmpUInt32(uint32_t in, std::string* out) {
  uint32_t ret = htobe32(in);
  out->append((char*) (&ret), sizeof(ret));
  return sizeof(ret);
}

size_t DecodeCmpUInt32(const std::string& in, uint32_t* out) {
  const uint32_t* p = reinterpret_cast<const uint32_t*>(in.data());
  *out = be32toh(*p);
  return sizeof(*out);
}
}
}// namespace amdb
