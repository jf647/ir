#include <gtest/gtest.h>

#include "indexer.h"

using std::cout;

TEST(TestIndexer, IndexSingleIntField) {
  auto indexer = Indexer();
  vector<IntField> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(IntField(i, "field1", i));
  }
  indexer.index(fields);
  cout << "done";
}
