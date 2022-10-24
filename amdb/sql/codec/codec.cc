#include "sql/codec/codec.h"

#include "common/log.h"

namespace amdb {
namespace codec {

size_t EncodeBytes(const std::string& in, std::string* out) {
  uint32_t len = in.size();
  size_t ret = EncodeUInt32(len, out);
  out->append(in.data(), in.size());
  return in.size() + ret;
}

size_t DecodeBytes(const std::string& in, std::string* out) {
  uint32_t len = 0;
  size_t ret = DecodeUInt32(in, &len);
  if (ret + len <= in.size()) {
    out->append(in.data() + ret, len);
    return len + ret;
  } else {
    return -1;
  }
}

size_t EncodeUInt8(uint8_t in, std::string* out) {
  out->append((char*)(&in), sizeof(in));
  return sizeof(*out);
}

size_t DecodeUInt8(const std::string& in, uint8_t* out) {
  *out = in.at(0);
  return sizeof(uint8_t);
}

size_t EncodeUInt32(uint32_t in, std::string* out) {
  uint32_t ret = htobe32(in);
  out->append((char*)(&ret), sizeof(ret));
  return sizeof(ret);
}

size_t DecodeUInt32(const std::string& in, uint32_t* out) {
  const uint32_t* p = reinterpret_cast<const uint32_t*>(in.data());
  *out = be32toh(*p);
  return sizeof(uint32_t);
}

size_t EncodeUInt64(uint64_t in, std::string* out) {
  uint64_t ret = htobe64(in);
  out->append((char*)(&ret), sizeof(ret));
  return sizeof(ret);
}

size_t DecodeUInt64(const std::string& in, uint64_t* out) {
  const uint64_t* p = reinterpret_cast<const uint64_t*>(in.data());
  *out = be64toh(*p);
  return sizeof(uint64_t);
}

size_t EncodeExprValue(const expr::ExprValue& in, std::string* out) {
  size_t size = 0;
  switch (in.Type()) {
    case expr::ExprValueType::UInt8:
      size = EncodeUInt8(in.u.uint8_value, out);
      break;
    case expr::ExprValueType::UInt32:
      size = EncodeUInt32(in.u.uint32_value, out);
      break;
    default:
      ERROR("Not Support type ", in.Type());
      break;
  }

  return size;
}

size_t DecodeExprValue(const std::string& in, expr::ExprValue* out) {
  size_t size = 0;
  switch (out->Type()) {
    case expr::ExprValueType::UInt8:
      size = DecodeUInt8(in, &out->u.uint8_value);
      break;
    case expr::ExprValueType::UInt32:
      size = DecodeUInt32(in, &out->u.uint32_value);
      break;
    default:
      ERROR("Not Support type ", out->Type());
      break;
  }
  return size;
}
}  // namespace codec
}  // namespace amdb