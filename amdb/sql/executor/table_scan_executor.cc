#include "sql/executor/table_scan_executor.h"

#include "common/log.h"

namespace amdb {
namespace executor {

Status TableScanExecutor::Open() { return Status::C_OK; }

Status TableScanExecutor::Close() { return Status::C_OK; }

Status TableScanExecutor::Generate(chunk::Chunk *chunk) {
  if (table_iter_ == nullptr) {
    table_iter_ = ctx_->arena->CreateObject<storage::TableIterator>(
        table_, 1024);
  }

  if (table_iter_ == nullptr) {
    ERROR("Failed to init table_iterator, table_id={}", table_info_->id);
    return Status::C_EXECUTOR_ERROR;
  }

  Status status = table_iter_->Open();
  RETURN_ERR_NOT_OK(status);
  table_iter_->Next();
  RETURN_ERR_NOT_OK(status);
  status = table_iter_->GetItem(chunk);
  RETURN_ERR_NOT_OK(status);
  return Status::C_OK;
}
}  // namespace executor
}  // namespace amdb