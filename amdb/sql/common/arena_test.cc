#include "sql/common/arena.h"

#include "gtest/gtest.h"

namespace amdb {
class ArenaTest : public testing::Test {
 protected:
  void SetUp() override {
    MemTracker tracker;
    arena_ = new Arena(&tracker);
  }

 protected:
  Arena* arena_;
};

struct Student {
  uint64_t id;

  enum Sex : unsigned char { male, female };
  Sex sex;
};

TEST_F(ArenaTest, CreateObject) {
  Student* stu1 = arena_->CreateObject<Student>();
  stu1->id = 0;
  stu1->sex = Student::Sex::male;

  /*
  Student* stu2 = arena_->CreateObject<Student>();
  stu2->id = 1;
  stu2->name = "Alice";
  stu2->sex = Student::Sex::female;
   */

  uint64_t size = arena_->SpaceAllocated();
  size = arena_->SpaceUsed();
  std::cout << size << std::endl;
}
}  // namespace amdb