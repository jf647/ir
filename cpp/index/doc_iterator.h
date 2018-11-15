#ifndef DOC_ITERATOR_H
#define DOC_ITERATOR_H

#include "cpp/query/query.h"

#include <boost/function_output_iterator.hpp>
#include <numeric>
#include <vector>

using std::iota;
using std::merge;
using std::sort;
using std::vector;

class DocWrapper {
private:
  vector<string> _docs;

public:
  DocWrapper(vector<string> docs) : _docs(docs) {}
  vector<string> docs() { return _docs; }
  DocWrapper &operator+=(const DocWrapper &other) {
    _docs.insert(_docs.end(), other._docs.begin(), other._docs.end());
    return *this;
  }
  virtual ~DocWrapper() {}
};
class ScoredDoc {

public:
  string docId;
  double score;
  ScoredDoc(string docId, double score) : docId(docId), score(score) {}
  ScoredDoc(string docId) : ScoredDoc(docId, 0) {}
  bool operator==(const ScoredDoc &other) const { return docId == other.docId; }
  bool operator<(const ScoredDoc &other) const { return docId < other.docId; }
  bool operator>(const ScoredDoc &other) const { return docId > other.docId; }
  ScoredDoc &operator+=(const ScoredDoc &other) {
    score += other.score;
    return *this;
  }
};
using ScoredDocs = vector<ScoredDoc>;
class Appender {
public:
  /* Cache the reference to our container. */
  Appender(ScoredDocs &docs) : docs(docs) {}

  /* Conditionally modify or append elements. */
  void operator()(const ScoredDoc &doc) const {
    if (!docs.empty() && docs.back() == doc) {
      docs.back() += doc;
      return;
    } // if
    docs.push_back(doc);
  }

private:
  /* Reference to our container. */
  ScoredDocs &docs;

}; // Appender
class DocCollector {
private:
  /* data */
  Combiner combiner;
  vector<ScoredDoc> combinedDocs;

  explicit DocCollector(Combiner combiner, vector<ScoredDoc> combinedDocs)
      : combiner(combiner), combinedDocs(combinedDocs) {}

  void collect(vector<ScoredDoc> docs) {
    sort(docs.begin(), docs.end());
    if (combinedDocs.size() == 0) {
      combinedDocs.insert(combinedDocs.end(), docs.begin(), docs.end());
      return;
    }
    ScoredDocs result;
    switch (combiner) {
    case SHOULD:
      merge(combinedDocs.begin(), combinedDocs.end(), docs.begin(), docs.end(),
            boost::make_function_output_iterator(Appender(result)));
      break;
    case MUST:
      auto first1 = combinedDocs.begin();
      auto last1 = combinedDocs.end();
      auto first2 = docs.begin();
      auto last2 = docs.end();
      while (first1 != last1 && first2 != last2) {
        if (*first1 == *first2) {
          result.push_back(*first1 += *first2);
          first1++;
          first2++;
        } else if (*first1 > *first2) {
          first2++;
        } else {
          first1++;
        }
      }
      break;
    }
    combinedDocs = result;
  }

public:
  explicit DocCollector(Combiner combiner) : DocCollector(combiner, {}) {}
  virtual ~DocCollector() {}

  void collect(vector<string> docs) {
    vector<ScoredDoc> tempDocs;
    tempDocs.insert(tempDocs.end(), docs.begin(), docs.end());
    collect(tempDocs);
  }
  void collect(vector<string> docs, vector<double> scores) {
    vector<ScoredDoc> tempDocs;
    for (int i = 0; i < docs.size(); i++) {
      tempDocs.push_back(ScoredDoc(docs[i], scores[i]));
    }
    collect(tempDocs);
  }
  void collect(const DocCollector &other) { collect(other.combinedDocs); }
  vector<string> elligible() const {
    vector<string> ell(combinedDocs.size());
    for (int i = 0; i < combinedDocs.size(); ++i)
      ell[i] = combinedDocs[i].docId;
    return ell;
  }
  vector<string> scoredResults() const {
    ScoredDocs temp = combinedDocs;
    sort(temp.begin(), temp.end(),
         [](ScoredDoc i, ScoredDoc j) { return i.score > j.score; });
    vector<string> ell(temp.size());
    for (int i = 0; i < temp.size(); ++i)
      ell[i] = temp[i].docId;
    return ell;
  }
};

#endif /* DOC_ITERATOR_H */
