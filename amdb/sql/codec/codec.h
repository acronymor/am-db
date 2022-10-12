#pragma

#include <string>

namespace amdb {
namespace codec {
size_t EncodeBytes(const std::string& in, std::string* out);
size_t DecodeBytes(const std::string& in, std::string* out);

size_t EncodeCmpUInt32(uint32_t in, std::string* out);
size_t DecodeCmpUInt32(const std::string& in, uint32_t* out);
}  // namespace codec
}  // namespace amdb