#include <gtest/gtest.h>

#include "searcher.h"

TEST(TestIntSearcher, RangedQuery) {
  auto indexer = Indexer();
  vector<IntField> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(IntField(i, "field1", i));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  auto q = make_shared<IntRangeQuery>("field1", 20, 40);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 20);
}
TEST(TestIntSearcher, MultiQueryMust) {
  auto indexer = Indexer();
  vector<IntField> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(IntField(i, "field1", i));
    fields.push_back(IntField(i, "field2", i % 10));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  vector<shared_ptr<Query>> queries;
  queries.push_back(make_shared<IntRangeQuery>("field1", 20, 40));
  queries.push_back(make_shared<IntRangeQuery>("field2", 2, 4));
  auto q = make_shared<NestedQuery>(queries, MUST);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 4);
}
TEST(TestIntSearcher, MultiQueryShould) {
  auto indexer = Indexer();
  vector<IntField> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(IntField(i, "field1", i));
    fields.push_back(IntField(i, "field2", i * 10));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  vector<shared_ptr<Query>> queries;
  queries.push_back(make_shared<IntRangeQuery>("field1", 20, 40));
  queries.push_back(make_shared<IntRangeQuery>("field2", 390, 410));
  auto q = make_shared<NestedQuery>(queries, SHOULD);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 21);
}
