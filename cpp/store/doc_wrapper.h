#ifndef DOC_WRAPPER_H
#define DOC_WRAPPER_H

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "proto/document.pb.h"

#include "cpp/field/fields.h"

using std::istream;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

using document::Document;

class DocWrapper {
private:
  Document doc;

public:
  string id();
  vector<shared_ptr<Field>> fields();
  DocWrapper(Document doc);
  virtual ~DocWrapper();
  void to_stream(ostream *to_stream);
};

DocWrapper from_stream(std::istream *from_stream);
#endif /* DOC_WRAPPER_H */
