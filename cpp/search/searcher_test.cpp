#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "searcher.h"

using std::to_string;
using testing::ElementsAre;

TEST(TestIntSearcher, RangedQuery) {

  IndexConfig config({make_shared<IntFieldConfig>("field1")});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(make_shared<IntField>(to_string(i), "field1", i));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  auto q = make_shared<IntRangeQuery>("field1", 20, 40);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 21);
}

TEST(TestIntSearcher, MultiQueryMust) {
  IndexConfig config({make_shared<IntFieldConfig>("field1"),
                      make_shared<IntFieldConfig>("field2")});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(make_shared<IntField>(to_string(i), "field1", i));
    fields.push_back(make_shared<IntField>(to_string(i), "field2", i % 10));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  vector<shared_ptr<Query>> queries;
  queries.push_back(make_shared<IntRangeQuery>("field1", 20, 40));
  queries.push_back(make_shared<IntRangeQuery>("field2", 2, 4));
  auto q = make_shared<NestedQuery>(queries, MUST);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 6);
}

TEST(TestIntSearcher, MultiQueryShould) {
  IndexConfig config({make_shared<IntFieldConfig>("field1"),
                      make_shared<IntFieldConfig>("field2")});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(make_shared<IntField>(to_string(i), "field1", i));
    fields.push_back(make_shared<IntField>(to_string(i), "field2", i * 10));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  vector<shared_ptr<Query>> queries;
  queries.push_back(make_shared<IntRangeQuery>("field1", 20, 40));
  queries.push_back(make_shared<IntRangeQuery>("field2", 390, 410));
  auto q = make_shared<NestedQuery>(queries, SHOULD);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 22);
}
TEST(TestStringSearcher, RangedQuery) {
  IndexConfig config({make_shared<StringFieldConfig>("field2")});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(make_shared<StringField>(to_string(i), "field2",
                                              std::to_string(i % 10)));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  auto q = make_shared<StringQuery>("field2", std::to_string(2));
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 10);
}
TEST(TestScoredStringSearcher, Query) {
  IndexConfig config(
      {make_shared<StringFieldConfig>("field2", SNOWBALL, true)});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  vector<vector<string>> s = {{"hello world"}, {"world not cool"}, {"cool"}};
  for (int i = 0; i < s.size(); ++i) {
    fields.push_back(make_shared<StringField>(to_string(i), "field2", s[i]));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  auto q = make_shared<StringQuery>("field2", "cool world", true);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 3);
  ASSERT_THAT(res, ElementsAre("1", "2", "0"));
}
TEST(TestVectorSearcher, Query) {
  IndexConfig config({make_shared<VectorFieldConfig>("field1", 3)});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  vector<vector<double>> s = {{1.0, 0.0, 0.0},
                              {0.0, 1.0, 0.0},
                              {0.0, 0.0, 1.0},
                              {1.0, 1.0, 1.0},
                              {-1.0, -1.0, -1.0}};
  for (int i = 0; i < s.size(); ++i) {
    fields.push_back(make_shared<VectorField>(to_string(i), "field1", s[i]));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  vector<double> query = {0, 1, 0};
  auto q = make_shared<VectorQuery>("field1", query, 10);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 5);
  ASSERT_THAT(res, ElementsAre("1", "3", "0", "2", "4"));
}

TEST(TestVectorSearcher, MultiQueryFiltered) {
  IndexConfig config({make_shared<IntFieldConfig>("field1"),
                      make_shared<VectorFieldConfig>("field2", 3)});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  vector<vector<double>> s = {{1.0, 0.0, 0.0},
                              {0.0, 1.0, 0.0},
                              {0.0, 0.0, 1.0},
                              {1.0, 1.0, 1.0},
                              {-1.0, -1.0, -1.0}};
  for (int i = 0; i < s.size(); i++) {
    fields.push_back(make_shared<IntField>(to_string(i), "field1", i));
    fields.push_back(make_shared<VectorField>(to_string(i), "field2", s[i]));
  }
  indexer.index(fields);
  Searcher s1(indexer);
  vector<double> query = {0, 1, 0};
  vector<shared_ptr<Query>> queries;
  queries.push_back(make_shared<IntRangeQuery>("field1", 2, 10));
  queries.push_back(make_shared<VectorQuery>("field2", query, 10));
  auto q = make_shared<NestedQuery>(queries, MUST);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 3);
  ASSERT_THAT(res, ElementsAre("3", "2", "4"));
}
TEST(TestApproximateVectorSearcher, Query) {
  IndexConfig config({make_shared<VectorFieldConfig>("field1", 3, 10, 1, 1)});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  vector<vector<double>> s = {{1.0, 0.10, 0.0},
                              {0.0, 1.1, 0.0},
                              {0.0, 0.0, 1.0},
                              {1.0, 1.0, 1.0},
                              {-1.0, -1.0, -1.0}};
  for (int i = 0; i < s.size(); ++i) {
    fields.push_back(make_shared<VectorField>(to_string(i), "field1", s[i]));
  }
  indexer.index(fields);
  indexer.refresh();
  Searcher s1(indexer);
  vector<double> query = {0, 1, 0};
  auto q = make_shared<VectorQuery>("field1", query, 5);
  auto res = s1.query(q);
  ASSERT_EQ(res.size(), 5);
  ASSERT_THAT(res, ElementsAre("1", "3", "0", "2", "4"));
}
