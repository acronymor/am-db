#pragma once

namespace amdb {
enum Status {
#define DECLARE_CODE(name, code) C_##name = code,
#include "sql/common/statuscode.inc"
#undef DECLARE_CODE
};

const char* GetErrorString(Status code);

}  // namespace amdb

#define unlikely(x) (__builtin_expect(!!(x), 0))

#define RETURN_ERR_NOT_OK(exp)            \
  do {                                    \
    Status code = (exp);                  \
    if (unlikely(code != Status::C_OK)) { \
      return code;                        \
    }                                     \
  } while (0)

#define RETURN_ERR_IF_TRUE(exp, code) \
  do {                                \
    if (unlikely(exp)) {              \
      return code;                    \
    }                                 \
  } while (0)

#define RETURN_IF_ERR_WITH_WARN(err, ctx, ...) \
  do {                                         \
    Status __err = (err);                      \
    if (unlikely(__err != Status::C_OK)) {     \
      (ctx)->E(WARNING_S(__VA_ARGS__));        \
      return __err;                            \
    }                                          \
  } while (0)
