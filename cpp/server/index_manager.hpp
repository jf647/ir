#ifndef INDEX_MANAGER_H
#define INDEX_MANAGER_H
#include <map>
#include <memory>
#include <string>

#include "cpp/index/indexer.h"
#include "cpp/store/file_store.h"

using std::map;
using std::shared_ptr;
using std::string;

class IndexManager final {
private:
  map<string, shared_ptr<Indexer>> indices;
  shared_ptr<FileStore> store;

public:
  IndexManager(shared_ptr<FileStore> store) : store(store) {}
  ~IndexManager() {}
  void Register(string indexName, shared_ptr<Indexer> indexer) {
    indices[indexName] = indexer;
    store->Register(indexer);
  }
  shared_ptr<Indexer> Get(string indexName) const {
    auto it = indices.find(indexName);
    if (it == indices.end()) {
      cout << "did not find index" << indexName << endl;
      return shared_ptr<Indexer>();
    }
    return it->second;
  }
};

#endif /* INDEX_MANAGER_H */
