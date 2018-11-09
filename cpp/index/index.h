#ifndef INDEX_H
#define INDEX_H

#include "cpp/ds/skiplist.h"
#include "cpp/field/field_base.h"

using std::static_pointer_cast;
using std::string;

class Index {
private:
  string _fieldName;

public:
  Index(string fieldName);
  string field_name();
  virtual void store(shared_ptr<Field> field) = 0;
};

class IntIndex : public Index {
  Skiplist<IntField> _index;

public:
  IntIndex(string fieldName);
  NodeRange<IntField> Find(IntField cursor) const;
  virtual void store(shared_ptr<Field> field);
};

Index::Index(string fieldName) : _fieldName(fieldName) {}
string Index::field_name() { return _fieldName; }
IntIndex::IntIndex(string fieldName) : Index(fieldName), _index(5, 0.5) {}
void IntIndex::store(shared_ptr<Field> field) {
  auto iField = static_pointer_cast<IntField>(field);
  _index.Insert(*iField);
}
NodeRange<IntField> IntIndex::Find(IntField cursor) const {
  return _index.Find(cursor);
}

#endif /* INDEX_H */
