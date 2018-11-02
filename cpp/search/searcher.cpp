#include "searcher.h"
#include <vector>
using std::string;
using std::vector;
Searcher::Searcher(shared_ptr<Index> index) : _index(index) {}

vector<int> Searcher::query(IntRangeQuery q) {
  NodeRange<IntField> startCursor = _index->Find(q.from());
  NodeRange<IntField> endCursor = _index->Find(q.to());
  vector<int> results;
  auto current = startCursor.begin();
  while (current != endCursor.begin()) {
    auto v = *current;
    results.push_back(v->Value().doc());
    ++current;
  }
  return results;
}
Searcher::~Searcher(){};
