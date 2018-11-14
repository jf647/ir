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
  const Searcher &_enclose;
  DocCollector collector;

public:
  explicit Executor(const Searcher &enclose, DocCollector collector)
      : _enclose(enclose), collector(collector) {}
  explicit Executor(const Searcher &enclose)
      : Executor(enclose, DocCollector(MUST)) {}
  vector<int> execute(shared_ptr<Query> q) {
    query(q);
    return collector.elligible();
  }
  void query(shared_ptr<Query> q) { q->accept(*this); }
  void visit(StringQuery *q) override {
    auto _index = _enclose.ir.get(q->fieldName());
    if (q->scored) {
      auto stringIndex = static_pointer_cast<ScoredStringIndex>(_index);
      auto results = stringIndex->Find(q->query());
      collector.collect(results);
    } else {
      auto stringIndex = static_pointer_cast<StringIndex>(_index);
      auto results = stringIndex->Find(q->query());
      auto r = results.docs();
      collector.collect(r);
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
    collector.collect(results);
  }
  void visit(VectorQuery *q) override {
    auto _index = _enclose.ir.get(q->fieldName());
    auto vectorIndex = static_pointer_cast<BaseVectorIndex>(_index);
    vectorIndex->sortBy(collector, q->query(), q->k());
  }
  void visit(NestedQuery *q) override {
    vector<int> tempDocs;
    DocCollector c = collector;
    switch (q->combiner()) {
    case SHOULD:
      c = DocCollector(SHOULD);
      break;
    case MUST:
      c = DocCollector(MUST);
      break;
    }
    for (shared_ptr<Query> query : q->queries()) {
      Executor e(_enclose, c);
      e.query(query);
      c = e.collector;
    }
    collector.collect(c.elligible());
  }
};

#endif /* SEARCHER_H */
