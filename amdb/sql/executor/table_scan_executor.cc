#include "sql/executor/table_scan_executor.h"

#include "common/log.h"
#include "sql/storage/bplus_tree_iterator.h"

namespace amdb {
namespace executor {

Status TableScanExecutor::Open() {
  for (auto& filter : scan_plan_->primary_filters) {
    RETURN_ERR_NOT_OK(filter->Open(ctx_));
  }
  for (auto& filter : scan_plan_->table_filters) {
    RETURN_ERR_NOT_OK(filter->Open(ctx_));
  }
  return Status::C_OK;
}

Status TableScanExecutor::Close() {
  for (auto& filter : scan_plan_->primary_filters) {
    RETURN_ERR_NOT_OK(filter->Close(ctx_));
  }
  for (auto& filter : scan_plan_->table_filters) {
    RETURN_ERR_NOT_OK(filter->Close(ctx_));
  }
  return Status::C_OK;
}

Status TableScanExecutor::Generate(chunk::Chunk* chunk) {
  if (table_->row_index == nullptr) {
    chunk->SetRowDesc(row_desc_);
    RETURN_ERR_NOT_OK(table_->Prepare());
  }

  if (table_iter_ == nullptr) {
    // TODO determine the upper limit of data processing
    table_iter_ = ctx_->arena->CreateObject<storage::TableIterator>(table_, 1024);
    for (const planner::IndexRange* range : primary_ranges_) {
      table_iter_->AddRange(range);
    }
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