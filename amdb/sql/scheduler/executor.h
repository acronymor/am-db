#pragma once

#include <vector>

#include "sql/common/statuscode.h"
#include "sql/scheduler/port.h"

namespace amdb {
namespace scheduler {
class IExecutor {
 public:
  enum ExecType { kExecShow, kExecTableScan, kExecFilter, kExecResultSet };

  enum State {
    kNeedData,
    kPortFull,
    kReady,
    kFinished,
  };

  IExecutor(ExecType exec_type) : exec_type_(exec_type){};

  virtual Status Open() { return Status::C_OK; }
  virtual Status Close() { return Status::C_OK; }
  virtual State Prepare() = 0;
  virtual State Work() = 0;

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

  ExecType exec_type_;
};
}  // namespace scheduler
}  // namespace amdb