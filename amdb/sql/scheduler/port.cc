#include "sql/scheduler/port.h"

namespace amdb {
namespace scheduler {

void Connect(InputPort& input, OutputPort& output) {
  output.input_port_ = &input;
  input.output_port_ = &output;
}
}
}