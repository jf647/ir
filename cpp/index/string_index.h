#ifndef STRING_INDEX_H
#define STRING_INDEX_H
#include "Eigen/Sparse"
#include <atomic>
#include <cmath>
#include <string>

#include "cpp/analyzer/snowball_analyzer.h"
#include "cpp/ceigen/eigen_mask.h"
#include "cpp/ds/skiplist.h"
#include "cpp/field/string_fields.h"

#include "doc_iterator.h"
#include "index.h"

using Eigen::Dynamic;
using Eigen::Matrix;
using Eigen::SparseMatrix;
using Eigen::SparseVector;

using std::atomic_int;
using std::sort;
using std::string;

class Token : public DocWrapper {
private:
  string _value;

public:
  explicit Token(string value, vector<string> docs)
      : DocWrapper(docs), _value(value) {}
  Token(int value) : Token("", {""}) {}
  Token(string value) : Token(value, {}) {}
  virtual ~Token() override {}
  virtual bool operator<(Token other) { return _value < other._value; }
  virtual bool operator==(Token other) { return _value == other._value; }
  // todo why wont utility above work
  virtual bool operator!=(Token other) { return _value != other._value; }
};

class StringIndex : public Index {
  Skiplist<Token> _index;
  AnalyzerType _analyzer;
  void store(Token token);

public:
  explicit StringIndex(string fieldName, AnalyzerType analyzer);
  DocWrapper Find(string cursor) const;
  virtual void store(shared_ptr<Field> field);
};
StringIndex::StringIndex(string fieldName, AnalyzerType analyzer)
    : Index(fieldName), _index(5, 0.5), _analyzer(analyzer) {}
void StringIndex::store(Token token) { _index.Insert(token); }
void StringIndex::store(shared_ptr<Field> field) {
  auto iField = static_pointer_cast<StringField>(field);
  for (auto token : iField->tokens()) {
    switch (_analyzer) {
    case SNOWBALL: {
      SnowballAnalyzer a;
      for (auto t : a.tokens(token))
        store(Token(t, {iField->doc()}));
      break;
    }
    case NONE:
      store(Token(token, {iField->doc()}));
    }
  }
}
DocWrapper StringIndex::Find(string query) const {
  vector<string> tokens;
  switch (_analyzer) {
  case SNOWBALL: {
    SnowballAnalyzer a;
    tokens = a.tokens(query);
    break;
  }
  case NONE:
    tokens = {query};
  }
  DocWrapper d({});
  for (auto token : tokens) {
    NodeRange<Token> wrappers = _index.Find(token);
    auto current = *wrappers;
    Token t = current->Value();
    d += t;
  }
  return d;
}
struct VocabNode {
  string _value;
  int _id;
  VocabNode(string value) : _value(value) {}
  VocabNode(int value) : VocabNode("") {}
  explicit VocabNode(string value, int id) : _value(value), _id(id) {}
  virtual bool operator<(VocabNode other) { return _value < other._value; }
  virtual bool operator==(VocabNode other) { return _value == other._value; }
  // todo why wont utility above work
  virtual bool operator!=(VocabNode other) { return _value != other._value; }
  VocabNode &operator+=(const VocabNode &other) { return *this; }
};
class Vocabulary {
  Skiplist<VocabNode> _index;
  int vocabCount;

public:
  Vocabulary() : _index(5, 0.5), vocabCount(-1) {}
  int fetch(string token) {
    auto id = fetchOrElse(token, -1);
    if (id != -1) {
      return id;
    }
    VocabNode n(token, ++vocabCount);
    return _index.Insert(n)->Value()._id;
  }
  int fetchOrElse(string token, int def) {
    auto node = *(_index.Find(token));
    if (node && node->Value()._value == token) {
      return node->Value()._id;
    }
    return def;
  }
};
class ScoredStringIndex : public Index {
  AnalyzerType _analyzer;
  shared_ptr<Vocabulary> vocab;
  SparseMatrix<double> terms;
  vector<string> docIds;
  atomic_int currDoc;

  void store(string token, int docId) {
    auto id = vocab->fetch(token);
    growCols(id);
    terms.coeffRef(docId, id)++;
  }
  void growRows() {
    if (currDoc == terms.rows()) {
      int newRows = terms.rows() * 2;
      terms.conservativeResize(newRows, terms.cols());
      docIds.resize(newRows);
    }
  }
  void growCols(int tokenId) {
    if (tokenId >= terms.cols()) {
      int newCols = terms.cols() * 2;
      terms.conservativeResize(terms.rows(), newCols);
    }
  }

public:
  explicit ScoredStringIndex(string fieldName, AnalyzerType analyzer)
      : Index(fieldName), _analyzer(analyzer), vocab(make_shared<Vocabulary>()),
        terms(10, 10), docIds(10), currDoc(0) {}
  void store(shared_ptr<Field> field) {
    growRows();
    auto docId = currDoc.fetch_add(1);
    docIds[docId] = field->doc();
    auto iField = static_pointer_cast<StringField>(field);
    for (auto token : iField->tokens()) {
      switch (_analyzer) {
      case SNOWBALL: {
        SnowballAnalyzer a;
        for (auto t : a.tokens(token))
          store(t, docId);
        break;
      }
      case NONE:
        store(token, docId);
      }
    }
  }
  void Find(DocCollector &collector, string query) const {
    vector<string> tokens;
    switch (_analyzer) {
    case SNOWBALL: {
      SnowballAnalyzer a;
      tokens = a.tokens(query);
      break;
    }
    case NONE:
      tokens = {query};
    }
    Matrix<bool, 1, Dynamic> m(1, terms.rows());
    for (auto token : tokens) {
      auto id = vocab->fetchOrElse(token, -1);
      if (id != -1)
        m.coeffRef(0, id) = true;
    }
    SparseVector<double> docTermCount(terms.cols());
    SparseVector<double> docFreq(terms.rows());
    for (int k = 0; k < terms.outerSize(); ++k)
      for (SparseMatrix<double>::InnerIterator it(terms, k); it; ++it) {
        if (it.value() > 0)
          docFreq.coeffRef(it.col(), 0) += 1;
        docTermCount.coeffRef(it.row(), 0) += 1;
      }
    double numDocs = terms.rows();
    for (SparseVector<double>::InnerIterator it(docFreq); it; ++it) {
      it.valueRef() = log10(1 + numDocs / it.value());
    }

    SparseMatrix<double> res = mask(terms, m);
    for (int k = 0; k < res.outerSize(); ++k)
      for (SparseMatrix<double>::InnerIterator it(res, k); it; ++it) {
        auto scaler =
            docFreq.coeff(it.col(), 0) / docTermCount.coeff(it.row(), 0);
        it.valueRef() *= scaler;
      }

    SparseVector<double> scores(numDocs);
    for (int k = 0; k < res.outerSize(); ++k)
      for (SparseMatrix<double>::InnerIterator it(res, k); it; ++it) {
        scores.coeffRef(it.row(), 0) += it.value();
      }
    vector<string> docs(scores.nonZeros());
    vector<double> scoresVec(scores.nonZeros());
    int i = 0;
    for (SparseVector<double>::InnerIterator it(scores); it; ++it) {
      docs[i] = docIds[it.row()];
      scoresVec[i] = it.value();
      ++i;
    }
    collector.collect(docs, scoresVec);
  }
};

#endif /* STRING_INDEX_H */
