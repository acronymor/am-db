#pragma once

#include "common/assert.h"
#include "sql/chunk/chunk.h"

namespace amdb {
namespace scheduler {

class IExecutor;

class InputPort;
class OutputPort;
void Connect(InputPort&, OutputPort&);
void Connect(OutputPort&, InputPort&);

class Port {
 private:
  friend void Connect(InputPort&, OutputPort&);
  friend void Connect(OutputPort&, InputPort&);

 public:
  enum State {
    kIsFinished,
    kNeedData,
    kHasData,
  };

  class Cache {
   public:
    void Push(chunk::Chunk* chunk);

    chunk::Chunk* Pull();

   private:
    chunk::Chunk* chunk_;
  };

 public:
  Port(IExecutor* exec = nullptr) : exec_(exec) {
    AMDB_ASSERT_TRUE(exec_ != nullptr);
  }

  virtual ~Port() = default;

  IExecutor* GetExecutor() const { return exec_; }
  void SetExecutor(IExecutor* exec) { exec_ = exec; }
  void SetIdx(size_t idx) { idx_ = idx; }
  size_t GetIdx() { return idx_; }

 protected:
  IExecutor* exec_ = nullptr;
  size_t idx_ = 0;
  std::shared_ptr<Cache> cache_;
};

class InputPort : public Port {
 private:
  friend void Connect(InputPort&, OutputPort&);
  friend void Connect(OutputPort&, InputPort&);

 public:
  explicit InputPort(IExecutor* exec) : Port(exec){};
  bool operator==(const InputPort& p) const;

  chunk::Chunk* PullData(bool set_not_needed = false);

  bool IsFinished() const;
  void SetNeeded();
  void SetNotNeeded();

  void Close();
  OutputPort* GetPeerPort() const { return output_port_; }

 private:
  OutputPort* output_port_{nullptr};
};

class OutputPort : public Port {
 private:
  friend void Connect(InputPort&, OutputPort&);
  friend void Connect(OutputPort&, InputPort&);

 public:
  explicit OutputPort(IExecutor* exec) : Port(exec) {}
  bool operator==(const OutputPort& p) const;

  void PushData(chunk::Chunk* data);

  bool IsFinished() const;
  bool CanPush() const;
  void Finish();

  InputPort* GetPeerPort() const { return input_port_; }

 private:
  InputPort* input_port_ = {nullptr};
};
}  // namespace scheduler
}  // namespace amdb