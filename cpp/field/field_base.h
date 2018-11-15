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
  string _docId;
  string _fieldName;

public:
  explicit Field(string docId, string fieldName)
      : _docId(docId), _fieldName(fieldName) {}

  string doc() { return _docId; }
  string field_name() { return _fieldName; }
  // Field &operator+=(const Field &other) {
  //  _docIds.insert(_docIds.end(), other._docIds.begin(), other._docIds.end());
  //  return *this;
  //}
  virtual ~Field(){};
  friend ostream &operator<<(ostream &os, const Field &field);
};

class IntField : public Field {
  int _value;

public:
  explicit IntField(string docId, string fieldName, int value)
      : Field(docId, fieldName), _value(value) {}
  IntField(int value) : IntField("", "", value) {}
  virtual bool operator<(IntField other) { return _value < other._value; }
  virtual bool operator==(IntField other) { return _value == other._value; }
  // todo why wont utility above work
  virtual bool operator!=(IntField other) { return _value != other._value; }
  int value() { return _value; }
};

ostream &operator<<(ostream &os, const Field &field) {
  os << "(Doc Id:";

  // Now add the last element with no delimiter
  os << field._docId;
  os << " Field Name:" << field._fieldName << ")";
  return os;
}

#endif /* FIELD_BASE_H */
