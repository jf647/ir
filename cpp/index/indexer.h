#ifndef INDEXER_H
#define INDEXER_H

#include <map>
#include <memory>
#include <vector>

#include "cpp/ds/skiplist.h"
#include "cpp/field/int_field.h"
#include "cpp/query/query.h"

using std::map;
using std::shared_ptr;
using std::vector;

class Index {
private:
  string _fieldName;
  Skiplist<IntField> _index;

public:
  Index(string fieldName);
  string field_name();
  void store(IntField field);
  NodeRange<IntField> Find(IntField cursor) const;
};

class Indexer {
private:
  map<string, shared_ptr<Index>> _fieldMap;

public:
  Indexer();
  void index(vector<IntField> fields);
  shared_ptr<Index> get(string fieldName) const;
  virtual ~Indexer();
};

Index::Index(string fieldName) : _fieldName(fieldName), _index(5, 0.5) {}
string Index::field_name() { return _fieldName; }
void Index::store(IntField field) { _index.Insert(field); }
NodeRange<IntField> Index::Find(IntField cursor) const {
  return _index.Find(cursor);
}

Indexer::Indexer() {}
void Indexer::index(vector<IntField> fields) {
  for (IntField field : fields) {
    auto it = _fieldMap.find(field.field_name());
    if (it == _fieldMap.end()) {
      _fieldMap[field.field_name()] = make_shared<Index>(field.field_name());
    }
    _fieldMap[field.field_name()]->store(field);
  }
}
shared_ptr<Index> Indexer::get(string fieldName) const {
  return _fieldMap.find(fieldName)->second;
}
Indexer::~Indexer() {}

#endif /* INDEXER_H */
