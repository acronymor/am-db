#pragma once

#include <vector>

#include "sql/common/statuscode.h"
#include "sql/scheduler/port.h"
#include "sql/context/statement_context.h"

namespace amdb {
namespace scheduler {
class IExecutor {
 public:
  enum Type { kExecShow, kExecTableScan, kExecFilter, kExecResultSet };

  enum State {
    kNeedData,
    kPortFull,
    kReady,
    kFinished,
  };

  explicit IExecutor(StatementContext* ctx, Type exec_type) : ctx_(ctx), type_(exec_type){};
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
  Type type_;
};
}  // namespace scheduler
}  // namespace amdb