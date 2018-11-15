#include "file_store.h"

#include <fstream>
#include <sys/stat.h>

using std::ifstream;
using std::ofstream;

FileStore::FileStore(Config c) : base_dir(c.base_dir()) {}

FileStore::~FileStore() {}

string filename(string base_dir, string id) { return base_dir + "/" + id; }

bool exists(string filename) {
  struct stat buffer;
  return stat(filename.c_str(), &buffer) == 0;
}

void FileStore::store(DocWrapper dw) {
  auto fn = filename(base_dir, dw.id());
  if (exists(fn))
    return;
  ofstream file;
  file.open(fn);
  dw.to_stream(&file);
  file.close();
}

DocWrapper FileStore::fetch(string id) {
  auto fn = filename(base_dir, id);
  ifstream file;
  file.open(fn);
  DocWrapper record = from_stream(&file);
  file.close();
  return record;
}
