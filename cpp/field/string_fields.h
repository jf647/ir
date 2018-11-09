#ifndef STRING_FIELDS_H
#define STRING_FIELDS_H

#include "cpp/analyzer/snowball_analyzer.h"
#include "field_base.h"

enum AnalyzerType { NONE, SNOWBALL };

class StringField : public Field {
  vector<string> _value;

public:
  explicit StringField(int docId, string fieldName, vector<string> value)
      : Field(docId, fieldName), _value(value) {}
  explicit StringField(int docId, string fieldName, string value)
      : Field(docId, fieldName), _value({value}) {}

  vector<string> tokens() { return _value; }
};

#endif /* STRING_FIELDS_H */
