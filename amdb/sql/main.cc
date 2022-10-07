#include "sql/main.h"

#include <gflags/gflags.h>

#include "common/log.h"
#include "common/version.h"
#include "sql/storage/leveldb_client.h"

namespace amdb {

int run(int argc, char* argv[]) {
  gflags::SetVersionString(GetRcsid());
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (init_log(argv[0]) != 0) {
    fprintf(stderr, "log init failed.");
    return -1;
  }

  storage::StorageAPIOptions options;
  storage::KvStorageAPI* api = storage::LevelDbClient::Create(options);
  Status status = api->PutKV("key", "value");
  std::string value;
  api->GetKV("key", &value);
  INFO("put kv {}", value);

  return 0;
}
}  // namespace amdb

int main(int argc, char* argv[]) { amdb::run(argc, argv); }