#ifndef INDEX_H
#define INDEX_H

#include "cpp/field/field_base.h"

using std::shared_ptr;
using std::static_pointer_cast;
using std::string;

class Index {
private:
  string _fieldName;

public:
  Index(string fieldName);
  string field_name();
  virtual void store(shared_ptr<Field> field) = 0;
  virtual void refresh() {}
};

Index::Index(string fieldName) : _fieldName(fieldName) {}
string Index::field_name() { return _fieldName; }

#endif /* INDEX_H */
