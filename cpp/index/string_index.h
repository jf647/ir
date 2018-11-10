#ifndef STRING_INDEX_H
#define STRING_INDEX_H
#include <cmath>

#include "cpp/field/string_fields.h"
#include "doc_iterator.h"
#include "index.h"

#include "Eigen/Sparse"
#include "cpp/ceigen/eigen_mask.h"

using std::sort;

class Token : public DocWrapper {
private:
  string _value;

public:
  explicit Token(string value, vector<int> docs)
      : DocWrapper(docs), _value(value) {}
  Token(int value) : Token("", {-1}) {}
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
        store(Token(t, iField->docs()));
      break;
    }
    case NONE:
      store(Token(token, iField->docs()));
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
  Eigen::SparseMatrix<double> terms;
  void store(string token, vector<int> docs) {
    auto id = vocab->fetch(token);
    growIf(id, docs);
    for (auto doc : docs) {
      terms.coeffRef(doc, id)++;
    }
  }
  void growIf(int tokenId, vector<int> docs) {
    bool grow = false;
    int newRows = terms.rows();
    int newCols = terms.cols();

    auto maxDocId = *(std::max_element(docs.begin(), docs.end()));
    if (maxDocId >= terms.rows()) {
      newRows *= 2;
      grow = true;
    }
    if (tokenId >= terms.cols()) {
      newCols *= 2;
      grow = true;
    }
    if (grow) {
      terms.conservativeResize(newRows, newCols);
    }
  }

public:
  explicit ScoredStringIndex(string fieldName, AnalyzerType analyzer)
      : Index(fieldName), _analyzer(analyzer), vocab(make_shared<Vocabulary>()),
        terms(10, 10) {}
  void store(shared_ptr<Field> field) {
    auto iField = static_pointer_cast<StringField>(field);
    for (auto token : iField->tokens()) {
      switch (_analyzer) {
      case SNOWBALL: {
        SnowballAnalyzer a;
        for (auto t : a.tokens(token))
          store(t, iField->docs());
        break;
      }
      case NONE:
        store(token, iField->docs());
      }
    }
  }
  vector<int> Find(string query) const {
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
    Eigen::Matrix<bool, 1, Eigen::Dynamic> m(1, terms.rows());
    for (auto token : tokens) {
      auto id = vocab->fetchOrElse(token, -1);
      if (id != -1)
        m.coeffRef(0, id) = true;
    }
    Eigen::SparseVector<double> docTermCount(terms.cols());
    Eigen::SparseVector<double> docFreq(terms.rows());
    for (int k = 0; k < terms.outerSize(); ++k)
      for (Eigen::SparseMatrix<double>::InnerIterator it(terms, k); it; ++it) {
        if (it.value() > 0)
          docFreq.coeffRef(it.col(), 0) += 1;
        docTermCount.coeffRef(it.row(), 0) += 1;
      }
    double numDocs = terms.rows();
    for (Eigen::SparseVector<double>::InnerIterator it(docFreq); it; ++it) {
      it.valueRef() = log10(1 + numDocs / it.value());
    }

    Eigen::SparseMatrix<double> res = mask(terms, m);
    for (int k = 0; k < res.outerSize(); ++k)
      for (Eigen::SparseMatrix<double>::InnerIterator it(res, k); it; ++it) {
        auto scaler =
            docFreq.coeff(it.col(), 0) / docTermCount.coeff(it.row(), 0);
        it.valueRef() *= scaler;
      }

    Eigen::SparseVector<double> scores(numDocs);
    for (int k = 0; k < res.outerSize(); ++k)
      for (Eigen::SparseMatrix<double>::InnerIterator it(res, k); it; ++it) {
        scores.coeffRef(it.row(), 0) += it.value();
      }
    vector<int> docs(scores.nonZeros());
    vector<double> scoresVec(scores.nonZeros());
    int i = 0;
    for (Eigen::SparseVector<double>::InnerIterator it(scores); it; ++it) {
      docs[i] = it.row();
      scoresVec[i] = it.value();
      ++i;
    }

    sort(docs.begin(), docs.end(),
         [&scoresVec](int i, int j) { return scoresVec[i] > scoresVec[j]; });
    return docs;
  }
};

#endif /* STRING_INDEX_H */
