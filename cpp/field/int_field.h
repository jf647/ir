#ifndef INT_FIELD_H
#define INT_FIELD_H

#include <string>
#include <utility> // for other operators

using std::ostream;
using std::string;

class IntField {
private:
  int _docId;
  string _fieldName;
  int _value;

public:
  IntField(int value) : _docId(-1), _fieldName(""), _value(value) {}
  explicit IntField(int docId, string fieldName, int value)
      : _docId(docId), _fieldName(fieldName), _value(value){};
  bool operator<(IntField other) { return _value < other._value; }
  bool operator==(IntField other) { return _value == other._value; }
  bool operator!=(IntField other) { return _value != other._value; }
  int doc() { return _docId; }
  string field_name() { return _fieldName; }
  virtual ~IntField(){};

  friend ostream &operator<<(ostream &os, const IntField &intField);
};
ostream &operator<<(ostream &os, const IntField &intField) {
  os << "(Doc Id:" << intField._docId << " Field Name:" << intField._fieldName
     << " Value:" << intField._value << ")";
  return os;
}

#endif /* INT_FIELD_H */
