#ifndef SEARCHER_H
#define SEARCHER_H

#include "cpp/index/indexer.h"
#include "cpp/query/query.h"

class Searcher {
private:
  shared_ptr<Index> _index;

public:
  Searcher(shared_ptr<Index> index);
  vector<int> query(IntRangeQuery range);
  virtual ~Searcher();
};

#endif /* SEARCHER_H */
