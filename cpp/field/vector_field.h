#ifndef VECTOR_FIELD_H
#define VECTOR_FIELD_H

#include "field_base.h"

class VectorField : public Field {
private:
  vector<double> _value;

public:
  explicit VectorField(int docId, string fieldName, vector<double> value)
      : Field(docId, fieldName), _value(value) {}
  virtual ~VectorField() {}
  vector<double> vec() { return _value; }
};

#endif /* VECTOR_FIELD_H */
