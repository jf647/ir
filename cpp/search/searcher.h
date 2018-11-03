#ifndef SEARCHER_H
#define SEARCHER_H

#include "cpp/index/indexer.h"
#include "cpp/query/query.h"

class Searcher {
private:
  const IndexReader &ir;
  class Executor;

public:
  Searcher(const IndexReader &reader);
  vector<int> query(shared_ptr<Query> range);
  virtual ~Searcher();
};
class Searcher::Executor : public QueryVisitor {
  vector<int> docs;
  const Searcher &_enclose;

public:
  Executor(const Searcher &enclose) : _enclose(enclose) {}
  vector<int> execute(shared_ptr<Query> q) {
    q->accept(*this);
    return docs;
  }
  void visit(IntRangeQuery *q) override {
    auto _index = _enclose.ir.get(q->fieldName());
    NodeRange<IntField> startCursor = _index->Find(q->from());
    NodeRange<IntField> endCursor = _index->Find(q->to());
    vector<int> results;
    auto current = startCursor.begin();
    while (current != endCursor.begin()) {
      auto v = *current;
      results.push_back(v->Value().doc());
      ++current;
    }
    std::copy(results.begin(), results.end(), std::back_inserter(docs));
  }
  void visit(NestedQuery *q) override {
    vector<int> tempDocs;
    for (shared_ptr<Query> query : q->queries()) {
      Executor e(_enclose);
      auto r = e.execute(query);
      if (tempDocs.size() == 0) {
        std::copy(r.begin(), r.end(), std::back_inserter(tempDocs));
        std::sort(tempDocs.begin(), tempDocs.end());
        continue;
      }
      vector<int> t;
      switch (q->combiner()) {
      case SHOULD:
        std::sort(r.begin(), r.end());
        std::set_union(tempDocs.begin(), tempDocs.end(), r.begin(), r.end(),
                       std::back_inserter(t));
        tempDocs = t;
        break;
      case MUST:
        std::sort(r.begin(), r.end());
        std::set_intersection(tempDocs.begin(), tempDocs.end(), r.begin(),
                              r.end(), std::back_inserter(t));
        tempDocs = t;
        break;
      }
    }
    docs = tempDocs;
  }
};

#endif /* SEARCHER_H */
