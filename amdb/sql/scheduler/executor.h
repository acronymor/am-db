#pragma once

#include <vector>

#include "sql/common/statuscode.h"
#include "sql/context/statement_context.h"
#include "sql/scheduler/port.h"

namespace amdb {
namespace scheduler {
enum ExecutorType {
  kExecShow,
  kExecTableScan,
  kExecFilter,
  kExecResultSet,
  kExecCreateDatabase,
  kExecCreateTable,
  kExecInsert
};

class IExecutor {
 public:
  enum State {
    kNeedData,
    kPortFull,
    kReady,
    kFinished,
  };

  explicit IExecutor(StatementContext* ctx, ExecutorType exec_type, plan::RelOptNode* node)
      : ctx_(ctx), type_(exec_type), physical_node_(node){};

  virtual ~IExecutor() = default;

  virtual std::string Name() const;

  virtual Status Open() { return Status::C_OK; }
  virtual Status Close() { return Status::C_OK; }
  virtual State Prepare() = 0;
  virtual Status Work() = 0;

  std::vector<InputPort>& GetInputs() { return inputs_; }
  std::vector<OutputPort>& GetOutputs() { return outputs_; }

  InputPort& CreateInputPort() {
    InputPort port(this);
    port.SetIdx(inputs_.size());
    inputs_.emplace_back(std::move(port));
    return inputs_.back();
  }
  OutputPort& CreateOutputPort() {
    OutputPort port(this);
    port.SetIdx(outputs_.size());
    outputs_.emplace_back(std::move(port));
    return outputs_.back();
  }

 protected:
  std::vector<InputPort> inputs_;
  std::vector<OutputPort> outputs_;

  StatementContext* ctx_;
  ExecutorType type_;

  plan::RelOptNode* physical_node_ = nullptr;
};
}  // namespace scheduler
}  // namespace amdb