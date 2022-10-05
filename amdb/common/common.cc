#include <gflags/gflags.h>

namespace amdb{
DEFINE_bool(enable_debug, true, "open DB_DEBUG log");
DEFINE_bool(enable_trace, true, "open SELF_TRACE log");
DEFINE_bool(servitysinglelog, true, "diff servity message in seperate logfile");
}  // namespace baikaldb