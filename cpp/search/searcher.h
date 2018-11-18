#ifndef SEARCHER_H
#define SEARCHER_H

#include "cpp/index/indexer.h"
#include "cpp/query/query.h"
#include <vector>
using std::static_pointer_cast;
using std::string;
using std::vector;

class Searcher {
private:
  const IndexReader &ir;
  class Executor;

public:
  Searcher(const IndexReader &reader);
  vector<string> query(shared_ptr<Query> range);
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
  vector<string> execute(shared_ptr<Query> q) {
    query(q);
    return collector.scoredResults();
  }
  void query(shared_ptr<Query> q) { q->accept(*this); }
  void visit(StringQuery *q) override {
    auto _index = _enclose.ir.get(q->fieldName());
    if (q->scored) {
      auto stringIndex = static_pointer_cast<ScoredStringIndex>(_index);
      stringIndex->Find(collector, q->query());
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
    intIndex->Find(collector, q->from(), q->to());
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
    default:
      c = DocCollector(MUST);
      break;
    }
    for (shared_ptr<Query> query : q->queries()) {
      Executor e(_enclose, c);
      if (!query) {
        continue;
      }
      e.query(query);
      c = e.collector;
    }
    collector.collect(c);
  }
};
Searcher::Searcher(const IndexReader &reader) : ir(reader) {}

vector<string> Searcher::query(shared_ptr<Query> q) {
  Executor e(*this);
  return e.execute(q);
}
Searcher::~Searcher(){};

#endif /* SEARCHER_H */
