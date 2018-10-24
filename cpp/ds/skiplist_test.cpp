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
  SimpleRange operator++() {
    cout << "called ++" << endl;
    return SimpleRange(++value);
  }
  int operator*() { return value; }
  bool operator!=(SimpleRange other) { return value != other.value; }
};
TEST(TestSimpleRange, Naive) {
  for (auto i : SimpleRange(0)) {
    cout << i << '\n';
  }
}

TEST(SkiplistTest, TestCreateAndInsert) {
  auto sl = Skiplist<int>(5, 0.5);
  cout << "created sl\n";

  for (int i = 0; i < 10; i++) {
    sl.Insert(i * 5);
  }
  cout << sl;
  for (int i = 0; i < 10; i++) {
    sl.Insert(i * 2);
  }
  cout << sl;
  for (auto n : sl.Find(4)) {
    cout << n->Value() << endl;
  }
}
