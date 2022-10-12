#include "gflags/gflags.h"

namespace amdb {
DEFINE_bool(enable_debug, true, "open DB_DEBUG log");
DEFINE_bool(enable_trace, true, "open SELF_TRACE log");
DEFINE_string(leveldb_dir, "/tmp/testdb", "leveldb directory");

DEFINE_uint32(bptree_max_node,  16 * (1 << 10), "the size of bptree node");
}  // namespace amdb