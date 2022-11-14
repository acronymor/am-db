#include "sql/codec/codec.h"

#include "common/assert.h"
#include "gtest/gtest.h"

namespace amdb {
namespace codec {
class CodecTest : public ::testing::Test {};

TEST_F(CodecTest, String) {
  std::string a = "abc";
  std::string b;
  std::string c;
  EncodeBytes(a, &b);
  DecodeBytes(b, &c);
  AMDB_ASSERT_EQ(a, c);

  a.clear();
  b.clear();
  c.clear();
  a = "012";
  EncodeBytes(a, &b);
  DecodeBytes(b, &c);
  AMDB_ASSERT_EQ(a, c);
}

TEST_F(CodecTest, Vector) {
  std::vector<std::string> vectors = {"01", "02", "03", "04"};
  std::string keys;

  for (std::string& s : vectors) {
    EncodeBytes(s, &keys);
  }

  size_t i = 0;
  uint32_t offset = 0;
  while (offset < keys.length()) {
    std::string value;
    uint32_t len = 0;
    DecodeUInt32(std::string(keys.data() + offset, sizeof(uint32_t)), &len);
    offset += DecodeBytes(std::string(keys.data() + offset, sizeof(uint32_t) + len), &value);
    AMDB_ASSERT_EQ(vectors.at(i++), value);
  }
}
}  // namespace codec
}  // namespace amdb