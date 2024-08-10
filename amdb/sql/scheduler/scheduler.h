#include <iostream>
#include <queue>

#include "sql/common/statuscode.h"
#include "sql/context/statement_context.h"
#include "sql/scheduler/executing_graph.h"

namespace amdb {
namespace scheduler {
Status ExecutePlan(StatementContext* stmt_ctx);

class Scheduler {
 public:
  explicit Scheduler(StatementContext* stmt_ctx) : ctx_(stmt_ctx), graph_(stmt_ctx) {}
  ~Scheduler() = default;

  Status Run();

 private:
  ExecutingGraph graph_;
  StatementContext* ctx_;

  struct Task {
    ExecutingGraph::Node* node;
    Status status;

    explicit Task(ExecutingGraph::Node* node) : node(node){};
  };

  std::queue<Task*> pending_tasks_;
  std::queue<Task*> executing_tasks_;
};
}  // namespace scheduler
}  // namespace amdb