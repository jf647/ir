#include <gtest/gtest.h>

#include "indexer.h"

using std::cout;

TEST(TestIndexer, IndexSingleIntField) {
  IndexConfig config({make_shared<IntFieldConfig>("field1")});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(make_shared<IntField>(i, "field1", i));
  }
  indexer.index(fields);
}
TEST(TestIndexer, IndexSingleStringField) {
  IndexConfig config({make_shared<StringFieldConfig>("field1")});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(make_shared<StringField>(i, "field1", std::to_string(i)));
  }
  indexer.index(fields);
}
TEST(TestStringAnalyzedIndexer, IndexSingleScoredStringField) {
  IndexConfig config(
      {make_shared<StringFieldConfig>("field1", SNOWBALL, true)});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    fields.push_back(make_shared<StringField>(i, "field1", std::to_string(i)));
  }
  indexer.index(fields);
}
TEST(TestVectorIndexer, IndexSingleVectorField) {
  IndexConfig config({make_shared<VectorFieldConfig>("field1", 3)});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    auto d = (double)i;
    vector<double> f = {d, d * 2, d * 3};
    fields.push_back(make_shared<VectorField>(i, "field1", f));
  }
  indexer.index(fields);
}
TEST(TestApproximateVectorIndexer, IndexSingleVectorField) {
  IndexConfig config({make_shared<VectorFieldConfig>("field1", 3, 1, 1, 1)});
  auto indexer = Indexer(config);
  vector<shared_ptr<Field>> fields;
  for (int i = 0; i < 100; ++i) {
    auto d = (double)i;
    vector<double> f = {d, d * 2, d * 3};
    fields.push_back(make_shared<VectorField>(i, "field1", f));
  }
  indexer.index(fields);
  indexer.refresh();
}
