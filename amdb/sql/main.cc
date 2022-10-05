#include "sql/main.h"

#include <gflags/gflags.h>

#include "common/log.h"
#include "common/version.h"

namespace amdb {

int run(int argc, char* argv[]) {
  if (init_log(argv[0]) != 0) {
    fprintf(stderr, "log init failed.");
    return -1;
  }

  std::string test = "Hello world";
  TRACE("content:->| {} ", test);
  DEBUG("content:->| {} ", test);
  INFO("content:->| {} ", test);
  WARN("content:->| {} ", test);
  ERROR("content:->| {} ", test);

  gflags::SetVersionString(GetRcsid());
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return 0;
}
}  // namespace amdb

int main(int argc, char* argv[]) { amdb::run(argc, argv); }