#ifndef FILE_STORE_H
#define FILE_STORE_H
#include "config.h"
#include "doc_wrapper.h"

#include <fstream>
#include <sys/stat.h>

#include "cpp/index/indexer.h"

class FileStore {
private:
  string base_dir;
  vector<shared_ptr<Indexer>> indexers;

public:
  explicit FileStore(Config c);
  virtual ~FileStore();
  void store(store::DocWrapper dw);
  store::DocWrapper fetch(string id);
  void Register(shared_ptr<Indexer> indexer);
};

using std::ifstream;
using std::ofstream;

FileStore::FileStore(Config c) : base_dir(c.base_dir()) {}

FileStore::~FileStore() {}

string filename(string base_dir, string id) {
  cout << base_dir << "id" << id << endl;
  return base_dir + "/" + id;
}

bool exists(string filename) {
  struct stat buffer;
  return stat(filename.c_str(), &buffer) == 0;
}

void FileStore::store(store::DocWrapper dw) {
  auto fn = filename(base_dir, dw.id());
  if (exists(fn))
    return;
  ofstream file;
  file.open(fn);
  dw.to_stream(&file);
  file.close();
  for (auto in : indexers) {
    if (!in) {
      continue;
    }
    in->index(dw.fields());
  }
}

store::DocWrapper FileStore::fetch(string id) {
  auto fn = filename(base_dir, id);
  ifstream file;
  file.open(fn);
  store::DocWrapper record = store::from_stream(&file);
  file.close();
  return record;
}
void FileStore::Register(shared_ptr<Indexer> indexer) {
  indexers.push_back(indexer);
}

#endif /* FILE_STORE_H */
