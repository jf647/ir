#include "skiplist.h"
#include <gtest/gtest.h>
using std::cout;
class SimpleRange {
private:
  int value;

public:
  SimpleRange(int v) : value(v) {}
  SimpleRange begin() { return *this; }
  SimpleRange end() { return SimpleRange(10); }
  SimpleRange operator++() { return SimpleRange(++value); }
  int operator*() { return value; }
  bool operator!=(SimpleRange other) { return value != other.value; }
};
TEST(TestSimpleRange, Naive) { SimpleRange(0); }

TEST(SkiplistTest, TestCreateAndInsert) {
  auto sl = Skiplist<int>(5, 0.5);

  for (int i = 0; i < 10; i++) {
    sl.Insert(i * 5);
  }
  for (int i = 0; i < 10; i++) {
    sl.Insert(i * 2);
  }
}

TEST(SkiplistTest, TestFindEmpty) {
  auto sl = Skiplist<int>(5, 0.5);
  auto res = sl.Find(5);
  ASSERT_NE(*res, nullptr);
  for (int i = 0; i < 5; i++) {
    auto right = (*res)->Right(i);
    ASSERT_EQ(right, nullptr);
  }
  auto right = (*res)->Right(5);
  ASSERT_EQ(right, nullptr);
}
