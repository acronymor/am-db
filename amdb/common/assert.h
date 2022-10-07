#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "common/log.h"

#define LIKELY(x) (__builtin_expect((x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))

#define AMDB_ASSERT_TRUE(c) \
  amdb::Assertion(__FILE__, __LINE__).Is(LIKELY(c), #c)
#define AMDB_ASSERT_FALSE(c) \
  amdb::Assertion(__FILE__, __LINE__).Is(LIKELY(!(c)), #c)
#define AMDB_ASSERT(c) amdb::Assertion(__FILE__, __LINE__).Is(LIKELY(c), #c)
#define AMDB_ASSERT_EQ(a, b) amdb::Assertion(__FILE__, __LINE__).IsEq((a), (b))
#define AMDB_ASSERT_NE(a, b) amdb::Assertion(__FILE__, __LINE__).IsNe((a), (b))
#define AMDB_ASSERT_GE(a, b) amdb::Assertion(__FILE__, __LINE__).IsGe((a), (b))
#define AMDB_ASSERT_GT(a, b) amdb::Assertion(__FILE__, __LINE__).IsGt((a), (b))
#define AMDB_ASSERT_LE(a, b) amdb::Assertion(__FILE__, __LINE__).IsLe((a), (b))
#define AMDB_ASSERT_LT(a, b) amdb::Assertion(__FILE__, __LINE__).IsLt((a), (b))

namespace amdb {

// The execution of an assertion.
class Assertion {
 public:
  Assertion(const char* f, int l)
      : ok_(true), fname_(f), line_(l), ss_(nullptr) {
    output_fatal_log_if_assertion_ = false;
  }
  Assertion(bool output_fatal, const char* f, int l)
      : ok_(true), fname_(f), line_(l), ss_(nullptr) {
    output_fatal_log_if_assertion_ = output_fatal;
  }

  ~Assertion() {
    if (UNLIKELY(!ok_)) {
      fprintf(stderr, "%s:%d:%s\n", fname_, line_, ss_->str().c_str());
      if (output_fatal_log_if_assertion_) {
        FatalLog();
      }
      abort();
    }
  }

  inline void FatalLog() { FATAL("{}", ss_->str()); }

#define SET_ASSERTION_STATUS              \
  ss_ = new std::stringstream();          \
  (*ss_) << " Assertion failure " << msg; \
  ok_ = false

  Assertion& Is(bool b, const char* msg) {
    if (UNLIKELY(!b)) {
      SET_ASSERTION_STATUS;
    }
    return *this;
  }

  Assertion& IsZero(int v, const char* msg) {
    if (UNLIKELY(v != 0)) {
      SET_ASSERTION_STATUS;
    }
    return *this;
  }
#undef SET_ASSERTION_STATUS

#define BINARY_OP(name, op)                             \
  template <class X, class Y>                           \
  Assertion& name(const X& x, const Y& y) {             \
    if (UNLIKELY(!(x op y))) {                          \
      ss_ = new std::stringstream();                    \
      (*ss_) << " failed: " << x << (" " #op " ") << y; \
      ok_ = false;                                      \
    }                                                   \
    return *this;                                       \
  }

  BINARY_OP(IsEq, ==)
  BINARY_OP(IsNe, !=)
  BINARY_OP(IsGe, >=)
  BINARY_OP(IsGt, >)
  BINARY_OP(IsLe, <=)
  BINARY_OP(IsLt, <)
#undef BINARY_OP

#define BINARY_STROP(name, func, op)                          \
  template <class X, class Y>                                 \
  Assertion& name(const X& x, const Y& y) {                   \
    if (UNLIKELY(!(func(x, y) op 0))) {                       \
      ss_ = new std::stringstream();                          \
      (*ss_) << #func " failed: " << x << (" " #op " ") << y; \
      ok_ = false;                                            \
    }                                                         \
    return *this;                                             \
  }

  BINARY_STROP(IsStrEq, strcmp, ==)
  BINARY_STROP(IsStrNe, strcmp, !=)
  BINARY_STROP(IsStrCaseEq, strcasecmp, ==)
  BINARY_STROP(IsStrCaseNe, strcasecmp, !=)

#undef BINARY_STROP

  // Attach the specified value to the error message if an error has occurred
  template <class V>
  Assertion& operator<<(const V& value) {
    if (UNLIKELY(!ok_)) {
      (*ss_) << " " << value;
    }
    return *this;
  }

 private:
  bool ok_;
  const char* fname_;
  int line_;
  std::stringstream* ss_;
  bool output_fatal_log_if_assertion_;
};

}  // namespace amdb
