#include "absl/random/random.h"
#include "sql/expression/expr_value.h"

namespace amdb {
namespace testutil {
class DataGen {
 public:
  DataGen() = default;
  virtual ~DataGen() = default;

  inline expr::ExprValue GenUint64Value() {
    return expr::ExprValue::NewUInt64(GenUint64());
  }

  inline expr::ExprValue GenInt64Value() {
    return expr::ExprValue::NewInt64(GenInt64());
  }

  inline expr::ExprValue GenUint32Value() {
    return expr::ExprValue::NewUInt32(GenUint32());
  }

  inline expr::ExprValue GenInt32Value() {
    return expr::ExprValue::NewInt32(GenInt32());
  }

  inline expr::ExprValue GenBoolValue() {
    return expr::ExprValue::NewBool(GenBool());
  }

  inline expr::ExprValue GenDoubleValue() {
    return expr::ExprValue::NewDouble(GenDouble());
  }

  inline expr::ExprValue GenStringValue(size_t len = 8) {
    return expr::ExprValue::NewString(GenString(len));
  }

 protected:
  virtual uint64_t GenUint64() = 0;

  virtual int64_t GenInt64() = 0;

  virtual uint32_t GenUint32() = 0;

  virtual int32_t GenInt32() = 0;

  virtual uint8_t GenUint8() = 0;

  virtual int8_t GenInt8() = 0;

  virtual bool GenBool() = 0;

  virtual double GenDouble() = 0;

  virtual std::string GenString(size_t len = 8) = 0;
};

class RandomDataGen : public DataGen {
 public:
  explicit RandomDataGen(uint64_t seed = 0) {
    auto my_seed = absl::MakeSeedSeq();
    gen_ = new absl::BitGen(my_seed);
  }

  ~RandomDataGen() override { delete gen_; };

 protected:
  uint64_t GenUint64() override;
  int64_t GenInt64() override;
  uint32_t GenUint32() override;
  int32_t GenInt32() override;
  uint8_t GenUint8() override;
  int8_t GenInt8() override;
  bool GenBool() override;
  double GenDouble() override;
  std::string GenString(size_t len);

 private:
  absl::BitGen* gen_;
};

class SeqDataGen : public DataGen {
 public:
  explicit SeqDataGen(uint64_t cur = 0) : cur_(cur){};
  ~SeqDataGen() override = default;

  uint64_t NextSeq(uint64_t next_step = 1);

 protected:
  uint64_t GenUint64() override;

  int64_t GenInt64() override;

  uint32_t GenUint32() override;

  int32_t GenInt32() override;

  uint8_t GenUint8() override;

  int8_t GenInt8() override;

  bool GenBool() override;

  double GenDouble() override;

  std::string GenString(size_t len);

 private:
  uint64_t cur_ = 0;
};
}  // namespace testutil
}  // namespace amdb