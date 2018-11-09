#ifndef INDEX_CONFIG_H
#define INDEX_CONFIG_H
#include "index.h"
#include "string_index.h"
#include <string>

using std::shared_ptr;
using std::string;

class FieldConfig {
  string _fieldName;

public:
  explicit FieldConfig(string fieldName) : _fieldName(fieldName) {}
  string fieldName() { return _fieldName; }
  virtual shared_ptr<Index> index() = 0;
  virtual ~FieldConfig() {}
};
class IntFieldConfig : public FieldConfig {
public:
  explicit IntFieldConfig(string fieldName) : FieldConfig(fieldName) {}
  shared_ptr<Index> index() { return make_shared<IntIndex>(fieldName()); }
  ~IntFieldConfig() {}
};
class StringFieldConfig : public FieldConfig {
  AnalyzerType analyzer;

public:
  explicit StringFieldConfig(string fieldName, AnalyzerType analyzer)
      : FieldConfig(fieldName), analyzer(analyzer) {}
  explicit StringFieldConfig(string fieldName)
      : StringFieldConfig(fieldName, NONE) {}
  shared_ptr<Index> index() {
    return make_shared<StringIndex>(fieldName(), analyzer);
  }
  ~StringFieldConfig() {}
};

class IndexConfig {
private:
  vector<shared_ptr<FieldConfig>> _fieldConfigs;

public:
  explicit IndexConfig(vector<shared_ptr<FieldConfig>> configs)
      : _fieldConfigs(configs){};
  vector<shared_ptr<FieldConfig>> fields() { return _fieldConfigs; }
  virtual ~IndexConfig(){};
};

#endif /* INDEX_CONFIG_H */
