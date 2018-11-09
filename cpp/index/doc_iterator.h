#ifndef DOC_ITERATOR_H
#define DOC_ITERATOR_H

#include <vector>

using std::vector;

class DocWrapper {
private:
  vector<int> _docs;

public:
  DocWrapper(vector<int> docs) : _docs(docs) {}
  vector<int> docs() { return _docs; }
  DocWrapper &operator+=(const DocWrapper &other) {
    _docs.insert(_docs.end(), other._docs.begin(), other._docs.end());
    return *this;
  }
  virtual ~DocWrapper() {}
};

#endif /* DOC_ITERATOR_H */
