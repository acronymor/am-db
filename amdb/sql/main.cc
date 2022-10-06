#include "sql/main.h"

#include <gflags/gflags.h>

#include "common/log.h"
#include "common/version.h"
#include "storage/kv_storage.h"

namespace amdb {

int run(int argc, char* argv[]) {
  if (init_log(argv[0]) != 0) {
    fprintf(stderr, "log init failed.");
    return -1;
  }

  storage::KvStorageAPI api;
  Status status = api.PutKV("key", "value");
  INFO("put kv {}", status);

  gflags::SetVersionString(GetRcsid());
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return 0;
}
}  // namespace amdb

int main(int argc, char* argv[]) { amdb::run(argc, argv); }