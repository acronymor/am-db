#include "sql/common/statuscode.h"

namespace amdb {
const char* GetErrorString(Status code) {
  switch (code) {
#define DECLARE_CODE(name, code) \
  case code:                     \
    return "STATUS_" #name;
#include "sql/common/statuscode.inc"
#undef DECLARE_CODE
  }
  return "STATUS_UNKNOWN";
}

}  // namespace amdb