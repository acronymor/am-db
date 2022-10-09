#include "sql/main.h"

#include <gflags/gflags.h>

#include "common/log.h"
#include "common/version.h"
#include "sql/storage/kv_storage_api.h"

namespace amdb {

int init(int argc, char* argv[]) {
  // init gflags
  gflags::SetVersionString(GetRcsid());
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // init log
  if (init_log(argv[0]) != 0) {
    FATAL("{}", "log init failed.")
    return -1;
  }

  // init storage
  storage::StorageAPIOptions options;
  Status status = storage::KvStorageAPISingleton::Init(options);
  if (Status::C_OK != status) {
    FATAL("{}", GetErrorString(status));
    return -1;
  }

  return 0;
}

int run() {
  storage::KvStorageAPI* storage = storage::KvStorageAPISingleton::GetInstance();
  Status status = storage->PutKV("key", "Hello world");
  if (Status::C_OK != status) {
    ERROR("{}", GetErrorString(status));
  }

  std::string value;
  storage->GetKV("key", &value);
  INFO("kv: {}", value);

  return 0;
}
}  // namespace amdb

int main(int argc, char* argv[]) {
  amdb::init(argc, argv);
  amdb::run();
}