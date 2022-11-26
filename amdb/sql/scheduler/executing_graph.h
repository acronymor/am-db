#include <iostream>
#include <unordered_map>

#include "sql/scheduler/executor.h"

#include "sql/executor/filter_executor.h"
#include "sql/executor/resultset_executor.h"
#include "sql/executor/table_scan_executor.h"

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
  const std::vector<std::unique_ptr<Node>>& Nodes() const { return nodes_; }

#ifdef AMDB_BUILD_TEST
  void TestInit(StatementContext* ctx, schema::TableInfo* table_info) {
    executor::TableScanExecutor* table_scan_executor = ctx->arena->CreateObject<executor::TableScanExecutor>(ctx, IExecutor::Type::kExecTableScan, table_info);
    executor::FilterExecutor* filter_executor = ctx->arena->CreateObject<executor::FilterExecutor>(ctx, IExecutor::Type::kExecFilter);
    executor::ResultSetExecutor* resultset_executor = ctx->arena->CreateObject<executor::ResultSetExecutor>(ctx, IExecutor::Type::kExecResultSet);

    executors_.reserve(3);
    executors_.emplace_back(table_scan_executor);
    executors_.emplace_back(filter_executor);
    executors_.emplace_back(resultset_executor);
  }
#endif

 private:
  std::unordered_map<IExecutor*, uint64_t> exec_to_id;
  std::vector<IExecutor*> executors_;
  std::vector<std::unique_ptr<Node>> nodes_;
  IExecutor* root_executor_ = nullptr;
};
}  // namespace scheduler
}  // namespace amdb