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
  Scheduler(StatementContext* stmt_ctx) : ctx_(stmt_ctx) {}
  ~Scheduler() = default;

#ifdef AMDB_BUILD_TEST
  Status TestRun(schema::TableInfo* table_info) {
    graph_.TestInit(ctx_, table_info);
    return Run();
  }
#endif
  Status Run();

 private:
  ExecutingGraph graph_;
  StatementContext* ctx_;

  struct Task {
    ExecutingGraph::Node* node;
    Status status;

    explicit Task(ExecutingGraph::Node* node) : node(node){};
  };

  std::queue<std::shared_ptr<Task>> pending_tasks_;
  std::queue<std::shared_ptr<Task>> executing_tasks_;
};
}  // namespace scheduler
}  // namespace amdb