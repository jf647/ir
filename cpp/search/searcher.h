#ifndef SEARCHER_H
#define SEARCHER_H

#include "cpp/index/indexer.h"
#include "cpp/query/query.h"
using std::static_pointer_cast;

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
  void visit(StringQuery *q) override {
    auto _index = _enclose.ir.get(q->fieldName());
    if (q->scored) {
      auto stringIndex = static_pointer_cast<ScoredStringIndex>(_index);
      auto results = stringIndex->Find(q->query());
      docs.insert(docs.end(), results.begin(), results.end());
    } else {
      auto stringIndex = static_pointer_cast<StringIndex>(_index);
      auto results = stringIndex->Find(q->query());
      auto r = results.docs();
      docs.insert(docs.end(), r.begin(), r.end());
    }
  }
  void visit(IntRangeQuery *q) override {
    auto _index = _enclose.ir.get(q->fieldName());
    auto intIndex = static_pointer_cast<IntIndex>(_index);
    NodeRange<IntField> startCursor = intIndex->Find(q->from());
    NodeRange<IntField> endCursor = intIndex->Find(q->to());
    vector<int> results;
    auto current = startCursor.begin();
    while (current != endCursor.begin()) {
      auto v = *current;
      auto docs = v->Value().docs();
      results.insert(results.end(), docs.begin(), docs.end());
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
