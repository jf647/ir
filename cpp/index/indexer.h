#ifndef INDEXER_H
#define INDEXER_H

#include <map>
#include <memory>
#include <vector>

#include "cpp/ds/skiplist.h"
#include "cpp/field/field_base.h"
#include "cpp/query/query.h"
#include "index.h"
#include "index_config.h"

using std::map;
using std::shared_ptr;
using std::vector;

class IndexReader {
public:
  virtual shared_ptr<Index> get(string fieldName) const = 0;
};
class Indexer : public IndexReader {
private:
  map<string, shared_ptr<Index>> _fieldMap;

public:
  Indexer(IndexConfig config);
  void index(vector<shared_ptr<Field>> fields);
  shared_ptr<Index> get(string fieldName) const;
  virtual ~Indexer();
};

Indexer::Indexer(IndexConfig config) {
  for (auto fc : config.fields()) {
    _fieldMap[fc->fieldName()] = fc->index();
  }
}
void Indexer::index(vector<shared_ptr<Field>> fields) {
  for (auto field : fields) {
    auto it = _fieldMap.find(field->field_name());
    if (it == _fieldMap.end()) {
      cout << "should throw error here" << endl;
      return;
      //_fieldMap[field.field_name()] = make_shared<Index>(field.field_name());
    }
    _fieldMap[field->field_name()]->store(field);
  }
}
shared_ptr<Index> Indexer::get(string fieldName) const {
  return _fieldMap.find(fieldName)->second;
}
Indexer::~Indexer() {}

#endif /* INDEXER_H */
