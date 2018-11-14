#ifndef INDEX_CONFIG_H
#define INDEX_CONFIG_H
#include "index.h"
#include "string_index.h"
#include "vector_index.h"
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
  bool scored;

public:
  explicit StringFieldConfig(string fieldName, AnalyzerType analyzer,
                             bool scored)
      : FieldConfig(fieldName), analyzer(analyzer), scored(scored) {}
  explicit StringFieldConfig(string fieldName, AnalyzerType analyzer)
      : StringFieldConfig(fieldName, analyzer, false) {}
  explicit StringFieldConfig(string fieldName)
      : StringFieldConfig(fieldName, NONE, false) {}
  shared_ptr<Index> index() {
    if (scored)
      return make_shared<ScoredStringIndex>(fieldName(), analyzer);
    return make_shared<StringIndex>(fieldName(), analyzer);
  }
  ~StringFieldConfig() {}
};
enum VectorInternal { NONE_IN, MRPT };
class VectorFieldConfig : public FieldConfig {
private:
  int vecLength;
  VectorInternal type;
  int numTrees, depth;
  float density;

public:
  explicit VectorFieldConfig(string fieldName, int vecLength, int numTrees,
                             int depth, int density)
      : FieldConfig(fieldName), vecLength(vecLength), type(MRPT),
        numTrees(numTrees), depth(depth), density(density) {}
  explicit VectorFieldConfig(string fieldName, int vecLength)
      : FieldConfig(fieldName), vecLength(vecLength), type(NONE_IN) {}
  shared_ptr<Index> index() {
    switch (type) {
    case MRPT:
      return make_shared<ApproximateVectorIndex>(fieldName(), vecLength,
                                                 numTrees, depth, density);
    default:
      return make_shared<VectorIndex>(fieldName(), vecLength);
    }
  }
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
