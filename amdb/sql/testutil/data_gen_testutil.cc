#include "sql/testutil/data_gen_testutil.h"

namespace amdb {
namespace testutil {
uint64_t RandomDataGen::GenUint64() { return absl::Uniform<uint64_t>(*gen_); }

int64_t RandomDataGen::GenInt64() { return GenUint64() * (GenBool() ? 1 : -1); }

uint32_t RandomDataGen::GenUint32() { return absl::Uniform<uint32_t>(*gen_); }

int32_t RandomDataGen::GenInt32() {
  return GenUint32() * (GenInt32() ? 1 : -1);
}

uint8_t RandomDataGen::GenUint8() { return absl::Uniform<uint8_t>(*gen_); }

int8_t RandomDataGen::GenInt8() { return GenUint8() * (GenBool() ? 1 : -1); }

bool RandomDataGen::GenBool() { return absl::Bernoulli(*gen_, 0.5); }

double RandomDataGen::GenDouble() {
  return GenInt64() / 7.0 * (GenBool() ? 1 : -1);
}

std::string RandomDataGen::GenString(size_t len) {
  std::string s;
  s.reserve(len);
  for (size_t i = 0; i < len; i++) {
    s.push_back(GenUint8() % 26 + 'A');
  }
  return s;
}

uint64_t SeqDataGen::NextSeq(uint64_t step) { return cur_ += step; }

uint64_t SeqDataGen::GenUint64() { return cur_; }

int64_t SeqDataGen::GenInt64() { return cur_; }

uint32_t SeqDataGen::GenUint32() { return cur_; }

int32_t SeqDataGen::GenInt32() { return cur_; }

uint8_t SeqDataGen::GenUint8() { return cur_; }

int8_t SeqDataGen::GenInt8() { return cur_; }

bool SeqDataGen::GenBool() { return cur_ % 2 == 1; }

double SeqDataGen::GenDouble() { return cur_ * 1.0; }

std::string SeqDataGen::GenString(size_t len) {
  return "data_gen_" + std::to_string(cur_);
}
}  // namespace testutil
}  // namespace amdb