#ifndef FILE_STORE_H
#define FILE_STORE_H
#include "config.h"
#include "doc_wrapper.h"

class FileStore {
private:
  string base_dir;

public:
  explicit FileStore(Config c);
  virtual ~FileStore();
  void store(DocWrapper dw);
  DocWrapper fetch(string id);
};

#endif /* FILE_STORE_H */
