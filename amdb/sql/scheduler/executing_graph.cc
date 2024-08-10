#include "sql/scheduler/executing_graph.h"

#include <algorithm>
#include <vector>

#include "sql/executor/executor_util.h"

namespace amdb {
namespace scheduler {
namespace {
IExecutor* ToExecutor(StatementContext* stmt_ctx, plan::RelOptNode* cur_node, std::vector<IExecutor*>* executors) {
  IExecutor* parent_executor = executor::ToExecutor(stmt_ctx, cur_node);
  for (plan::RelOptNode* node : cur_node->GetInputs()) {
    IExecutor* child_exec = ToExecutor(stmt_ctx, node, executors);
    Connect(parent_executor->CreateInputPort(), child_exec->CreateOutputPort());
  }
  executors->push_back(parent_executor);
  return parent_executor;
}

ExecutingGraph::State StateConv(IExecutor::State state) {
  ExecutingGraph::State res;
  switch (state) {
    case IExecutor::State::kNeedData:
    case IExecutor::State::kPortFull:
      res = ExecutingGraph::State::kIdle;
      break;
    case IExecutor::State::kReady:
      res = ExecutingGraph::State::kExecuting;
      break;
    case IExecutor::State::kFinished:
      res = ExecutingGraph::State::kFinished;
      break;
  }
  return res;
}
}  // namespace

ExecutingGraph* ExecutingGraph::initExecutors() {
  plan::RelOptNode* root = stmt_ctx->physical_plan;
  ToExecutor(stmt_ctx, root, &executors_);
  return this;
}

ExecutingGraph* ExecutingGraph::initNodes() {
  // construct node
  nodes_.reserve(executors_.size());
  for (size_t id = 0; id < executors_.size(); id++) {
    IExecutor* executor = executors_.at(id);
    exec_to_id[executor] = id;
    nodes_.emplace_back(stmt_ctx->arena->CreateObject<Node>(executor, id));
  }

  return this;
}

ExecutingGraph* ExecutingGraph::initEdges() {
  // construct edge
  for (size_t id = 0; id < nodes_.size(); id++) {
    IExecutor* executor = nodes_.at(id)->exec;

    // backward edges, OutputPort -> [InputPort]
    std::vector<InputPort>& input_ports = executor->GetInputs();
    nodes_[id]->backward_edges.reserve(input_ports.size());

    for (size_t idx = 0; idx < input_ports.size(); idx++) {
      OutputPort* output_port = input_ports.at(idx).GetPeerPort();
      IExecutor* backward_connected_exec = output_port->GetExecutor();
      std::vector<OutputPort>& peer_outputs = backward_connected_exec->GetOutputs();
      size_t to_idx = std::find(peer_outputs.begin(), peer_outputs.end(), *output_port) - peer_outputs.begin();
      AMDB_ASSERT(to_idx == output_port->GetIdx());
      AMDB_ASSERT(exec_to_id.find(backward_connected_exec) != exec_to_id.end());
      Edge new_edge = {exec_to_id[backward_connected_exec], true};
      nodes_[id]->backward_edges.push_back(new_edge);
    }

    // directed edges, [OutputPort] -> InputPort
    std::vector<OutputPort>& output_ports = executor->GetOutputs();
    nodes_[id]->directed_edges.reserve(output_ports.size());

    for (size_t idx = 0; idx < output_ports.size(); idx++) {
      InputPort* input_port = output_ports.at(idx).GetPeerPort();
      IExecutor* forward_connected_exec = input_port->GetExecutor();
      std::vector<InputPort>& peer_inputs = forward_connected_exec->GetInputs();
      size_t to_idx = std::find(peer_inputs.begin(), peer_inputs.end(), *input_port) - peer_inputs.begin();
      AMDB_ASSERT(to_idx == input_port->GetIdx());
      AMDB_ASSERT(exec_to_id.find(forward_connected_exec) != exec_to_id.end());
      Edge new_edge = {exec_to_id[forward_connected_exec], false};
      nodes_[id]->directed_edges.push_back(new_edge);
    }
  }
  return this;
}

Status ExecutingGraph::Init() {
  this->initExecutors()->initNodes()->initEdges();
  return Status::C_OK;
}

Status ExecutingGraph::Finish() { return Status::C_OK; }

Status ExecutingGraph::Open() {
  for (int32_t i = nodes_.size() - 1; i >= 0; i--) {
    IExecutor* executor = nodes_.at(i)->exec;
    Status status = executor->Open();
    if (status != Status::C_OK) {
      ERROR("{}' executor open failed", executor->Name());
      return status;
    }
  }
  return Status::C_OK;
}

Status ExecutingGraph::Close() {
  for (int32_t i = nodes_.size() - 1; i >= 0; i--) {
    IExecutor* executor = nodes_.at(i)->exec;
    Status status = executor->Close();
    if (status != Status::C_OK) {
      ERROR("{}' executor close failed", executor->Name());
      return status;
    }
  }
  return Status::C_OK;
}

Status ExecutingGraph::Prepare(Node* node) {
  // TODO nodes can't be iterated by order, but based on graph order
  IExecutor* executor = node->exec;
  IExecutor::State state = executor->Prepare();
  node->data_state = state;
  node->running_state = StateConv(state);
  INFO("{} executor state is {}", executor->Name(), node->data_state);
  return Status::C_OK;
}

Status ExecutingGraph::Work(Node* node) {
  IExecutor* executor = node->exec;
  Status status = executor->Work();
  if (status != Status::C_OK) {
    ERROR("executor of {} close failed, status={}", executor->Name(), status);
    return status;
  }
  return Status::C_OK;
}
}  // namespace scheduler
}  // namespace amdb