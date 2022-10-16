#include "sql/common/arena.h"

#include "common/assert.h"
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

  uint64_t size = arena_->SpaceUsed();
  AMDB_ASSERT_LT(0, size);

  size = arena_->SpaceAllocated();
  AMDB_ASSERT_LT(0, size);
}
}  // namespace amdb