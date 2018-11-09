#ifndef FIELD_BASE_H
#define FIELD_BASE_H

#include <iostream>
#include <string>
#include <utility> // for other operators
#include <vector>

using std::cout;
using std::ostream;
using std::string;
using std::vector;

class Field {
private:
  vector<int> _docIds;
  string _fieldName;

public:
  explicit Field(int docId, string fieldName)
      : _docIds({docId}), _fieldName(fieldName) {}

  vector<int> docs() { return _docIds; }
  string field_name() { return _fieldName; }
  Field &operator+=(const Field &other) {
    _docIds.insert(_docIds.end(), other._docIds.begin(), other._docIds.end());
    return *this;
  }
  virtual ~Field(){};
  friend ostream &operator<<(ostream &os, const Field &field);
};

class IntField : public Field {
  int _value;

public:
  explicit IntField(int docId, string fieldName, int value)
      : Field(docId, fieldName), _value(value) {}
  IntField(int value) : IntField(-1, "", value) {}
  virtual bool operator<(IntField other) { return _value < other._value; }
  virtual bool operator==(IntField other) { return _value == other._value; }
  // todo why wont utility above work
  virtual bool operator!=(IntField other) { return _value != other._value; }
};

ostream &operator<<(ostream &os, const Field &field) {
  os << "(Doc Id:";
  std::copy(field._docIds.begin(), field._docIds.end() - 1,
            std::ostream_iterator<int>(os, ","));

  // Now add the last element with no delimiter
  os << field._docIds.back();
  os << " Field Name:" << field._fieldName << ")";
  return os;
}

#endif /* FIELD_BASE_H */
