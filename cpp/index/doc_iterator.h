#ifndef DOC_ITERATOR_H
#define DOC_ITERATOR_H

#include "cpp/query/query.h"

#include <vector>

using std::vector;

class DocWrapper {
private:
  vector<int> _docs;

public:
  DocWrapper(vector<int> docs) : _docs(docs) {}
  vector<int> docs() { return _docs; }
  DocWrapper &operator+=(const DocWrapper &other) {
    _docs.insert(_docs.end(), other._docs.begin(), other._docs.end());
    return *this;
  }
  virtual ~DocWrapper() {}
};
class DocCollector {
private:
  /* data */
  Combiner combiner;
  vector<int> combinedDocs;

  explicit DocCollector(Combiner combiner, vector<int> combinedDocs)
      : combiner(combiner), combinedDocs(combinedDocs) {}

public:
  explicit DocCollector(Combiner combiner) : DocCollector(combiner, {}) {}
  virtual ~DocCollector() {}

  void collect(vector<int> docs) {
    vector<int> tempDocs;
    if (combinedDocs.size() == 0) {
      combinedDocs = docs;
      return;
    }
    switch (combiner) {
    case SHOULD:
      std::sort(docs.begin(), docs.end());
      std::set_union(combinedDocs.begin(), combinedDocs.end(), docs.begin(),
                     docs.end(), std::back_inserter(tempDocs));
      break;
    case MUST:
      std::sort(docs.begin(), docs.end());
      std::set_intersection(combinedDocs.begin(), combinedDocs.end(),
                            docs.begin(), docs.end(),
                            std::back_inserter(tempDocs));
      break;
    }
    combinedDocs = tempDocs;
  }
  void collect(vector<int> docs, vector<double> scores) {
    vector<int> idx(docs.size());
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(),
         [&scores](int i, int j) { return scores[i] > scores[j]; });
    combinedDocs = vector<int>(docs.size());
    for (int i = 0; i < idx.size(); i++) {
      combinedDocs[i] = docs[idx[i]];
    }
  }
  vector<int> elligible() const { return combinedDocs; }
};

#endif /* DOC_ITERATOR_H */
