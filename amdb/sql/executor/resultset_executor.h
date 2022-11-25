#include "sql/scheduler/sink.h"

namespace amdb {
namespace executor {
class ResultSetExecutor : public scheduler::ISink {
 public:
  ResultSetExecutor(StatementContext* ctx, Type exec_type) : ISink(ctx, exec_type){};

  std::string Name() const override {
    return "ResultSet";
  };

  Status Open() override;
  Status Close() override;

 protected:
  Status Consume(chunk::Chunk* chunk) override;
};
}  // namespace executor
}  // namespace amdb