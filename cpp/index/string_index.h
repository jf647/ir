#ifndef STRING_INDEX_H
#define STRING_INDEX_H
#include "cpp/field/string_fields.h"
#include "doc_iterator.h"
#include "index.h"

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

#endif /* STRING_INDEX_H */
