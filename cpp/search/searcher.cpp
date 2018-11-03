#include "searcher.h"
#include <vector>
using std::string;
using std::vector;
Searcher::Searcher(const IndexReader &reader) : ir(reader) {}

vector<int> Searcher::query(shared_ptr<Query> q) {
  Executor e(*this);
  return e.execute(q);
}
Searcher::~Searcher(){};
