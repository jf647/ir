#include <gtest/gtest.h>

#include "searcher.h"

TEST(TestIntSearcher, RangedQuery) {
  auto indexer = Indexer();
  vector<IntField> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(IntField(i, "field1", i));
  }
  indexer.index(fields);
  Searcher s1(indexer.get("field1"));
  auto res = s1.query(IntRangeQuery(20, 40));
  ASSERT_EQ(res.size(), 20);
}
