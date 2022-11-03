#include "sql/scheduler/port.h"

namespace amdb {
namespace scheduler {
void Connect(InputPort& input, OutputPort& output) {
  input.output_port_ = &output;
  output.input_port_ = &input;
  input.cache_ = std::make_shared<Port::Cache>();
  output.cache_ = input.cache_;
}

void Connect(OutputPort& output, InputPort& input) { Connect(input, output); }

void Port::Cache::Push(chunk::Chunk* chunk) { chunk_ = chunk; }

chunk::Chunk* Port::Cache::Pull() { return chunk_; }

void OutputPort::PushData(chunk::Chunk* data) {
  cache_->Push(data);
}

chunk::Chunk* InputPort::PullData(bool set_not_needed) {
  return cache_->Pull();
}

bool OutputPort::IsFinished() const { return false; }
bool OutputPort::CanPush() const { return false; }
void OutputPort::Finish() {}
}  // namespace scheduler
}  // namespace amdb