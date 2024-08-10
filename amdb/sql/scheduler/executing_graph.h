#include <iostream>
#include <unordered_map>

#include "sql/executor/filter_executor.h"
#include "sql/executor/resultset_executor.h"
#include "sql/executor/table_scan_executor.h"
#include "sql/scheduler/executor.h"

namespace amdb {
namespace scheduler {

class ExecutingGraph {
 public:
  enum class State {
    kIdle,
    kPreparing,
    kExecuting,
    kFinished,
  };

  struct Edge {
    Edge(uint64_t to, bool backward) : to(to), backward(backward){};

    uint64_t to = -1;
    /* backward tells the direction of data flow.
     * backward == true, the edge outputs data
     * backward == false, the edge possess data to be read.
     */
    bool backward = false;
  };

  struct Node {
    Node(IExecutor* exec, uint64_t id) : exec(exec), id(id) {}

    std::string Name() const { return exec->Name(); }

    IExecutor* exec = nullptr;
    uint64_t id = 0;
    // a directed edge relates to an output port
    std::vector<Edge> directed_edges;
    // a backward edge relates to an input port
    std::vector<Edge> backward_edges;
    State running_state = State::kIdle;
    IExecutor::State data_state = IExecutor::State::kNeedData;
  };

 public:
  Status Init();
  Status Finish();
  Status Open();
  Status Close();
  Status Prepare(Node* node);
  Status Work(Node* node);

 public:
  explicit ExecutingGraph(StatementContext* ctx) : stmt_ctx(ctx){};

 private:
  ExecutingGraph* initExecutors();
  ExecutingGraph* initNodes();
  ExecutingGraph* initEdges();

 public:
  const std::vector<Node*>& Nodes() const { return nodes_; }

 private:
  StatementContext* stmt_ctx;

  std::unordered_map<IExecutor*, uint64_t> exec_to_id;
  std::vector<IExecutor*> executors_;
  std::vector<Node*> nodes_;
  IExecutor* root_executor_ = nullptr;
};
}  // namespace scheduler
}  // namespace amdb