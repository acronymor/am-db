#pragma once

namespace amdb {
enum Status {
#define DECLARE_CODE(name, code) C_##name = code,
#include "sql/common/statuscode.inc"
#undef DECLARE_CODE
};

const char* GetErrorString(Status code);

}  // namespace amdb
