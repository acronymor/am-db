#include "sql/codec/codec.h"

#include "common/assert.h"
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

size_t EncodeInt64(int64_t in, std::string* out) {
  uint64_t ret = htobe64(in);
  out->append((char*)(&ret), sizeof(ret));
  return sizeof(ret);
}

size_t DecodeInt64(const std::string& in, int64_t* out) {
  const int64_t* p = reinterpret_cast<const int64_t*>(in.data());
  *out = be64toh(*p);
  return sizeof(int64_t);
}

size_t EncodeExprValue(const expr::ExprValue& in, std::string* out) {
  size_t size = 0;
  switch (in.Type()) {
    case expr::ExprValueType::UInt8:
      size = EncodeUInt8(in.UInt8Value(), out);
      break;
    case expr::ExprValueType::Int8:
      size = EncodeUInt8(in.Int8Value(), out);
      break;
    case expr::ExprValueType::UInt32:
      size = EncodeUInt32(in.UInt32Value(), out);
      break;
    case expr::ExprValueType::Int32:
      size = EncodeUInt32(in.Int32Value(), out);
      break;
    case expr::ExprValueType::UInt64:
      size = EncodeUInt64(in.UInt64Value(), out);
      break;
    case expr::ExprValueType::Int64:
      size = EncodeUInt64(in.Int64Value(), out);
      break;
    case expr::ExprValueType::String:
      size = EncodeBytes(in.StringValue(), out);
      break;
    default:
      ERROR("Not Support type ", in.Type());
      break;
  }

  return size;
}

size_t DecodeExprValue(const std::string& in, expr::ExprValue* out, Arena* arena) {
  size_t size = 0;
  switch (out->Type()) {
    case expr::ExprValueType::UInt8: {
      std::string tmp;
      tmp.assign(in.data(), sizeof(uint8_t));
      size = DecodeUInt8(tmp, &out->u.uint8_value);
    } break;
    case expr::ExprValueType::UInt32: {
      std::string tmp;
      tmp.assign(in.data(), sizeof(uint32_t));
      size = DecodeUInt32(tmp, &out->u.uint32_value);
    } break;
    case expr::ExprValueType::Int64: {
      std::string tmp;
      tmp.assign(in.data(), sizeof(int64_t));
      size = DecodeUInt64(tmp, &out->u.uint64_value);
    } break;
    case expr::ExprValueType::UInt64: {
      std::string tmp;
      tmp.assign(in.data(), sizeof(uint64_t));
      size = DecodeUInt64(tmp, &out->u.uint64_value);
    } break;
    case expr::ExprValueType::String: {
      AMDB_ASSERT_FALSE(arena == nullptr);
      std::string value;
      size = DecodeBytes(in, &value);
      *out = expr::ExprValue::NewString(std::move(value), arena);
    } break;
    default:
      ERROR("Not Support type, {}", out->Type());
      break;
  }
  return size;
}
}  // namespace codec
}  // namespace amdb