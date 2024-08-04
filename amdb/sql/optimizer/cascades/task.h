#pragma once

#include <memory>
#include <string>
#include <vector>

namespace amdb {
namespace opt {
struct Cascades;
class Task {
 public:
  virtual ~Task() = default;

  virtual std::vector<std::unique_ptr<Task>> execute(Cascades* optimizer) = 0;

  virtual std::string describe() = 0;
};

}  // namespace opt
}  // namespace amdb
