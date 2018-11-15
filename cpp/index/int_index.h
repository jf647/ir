#ifndef INT_INDEX_H
#define INT_INDEX_H

#include "cpp/ds/skiplist.h"
#include "doc_iterator.h"
#include "index.h"

class IntIndex : public Index {
  class Token : public DocWrapper {
  private:
    int _value;

  public:
    explicit Token(int value, vector<string> docs)
        : DocWrapper(docs), _value(value) {}
    Token(int value) : Token(value, {""}) {}
    virtual ~Token() override {}
    virtual bool operator<(Token other) { return _value < other._value; }
    virtual bool operator==(Token other) { return _value == other._value; }
    // todo why wont utility above work
    virtual bool operator!=(Token other) { return _value != other._value; }
  };
  Skiplist<Token> _index;

public:
  IntIndex(string fieldName);
  void Find(DocCollector &collector, IntField from, IntField to) const;
  virtual void store(shared_ptr<Field> field);
};
IntIndex::IntIndex(string fieldName) : Index(fieldName), _index(5, 0.5) {}
void IntIndex::store(shared_ptr<Field> field) {
  auto iField = static_pointer_cast<IntField>(field);
  _index.Insert(Token(iField->value(), {iField->doc()}));
}
void IntIndex::Find(DocCollector &collector, IntField from, IntField to) const {
  auto begin = _index.Find(Token(from.value()));
  auto end = _index.Find(Token(to.value()));
  vector<string> res;
  auto current = begin.begin();
  while (current != end.begin()) {
    auto t = (*current)->Value().docs();
    res.insert(res.end(), t.begin(), t.end());
    ++current;
  }
  collector.collect(res);
}

#endif /* INT_INDEX_H */
