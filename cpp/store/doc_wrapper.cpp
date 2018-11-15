#include "doc_wrapper.h"

using std::make_shared;

DocWrapper::DocWrapper(Document doc) { doc.CopyFrom(doc); }

DocWrapper::~DocWrapper() {}

string DocWrapper::id() { return doc.id(); }

vector<shared_ptr<Field>> DocWrapper::fields() {
  vector<shared_ptr<Field>> fs;
  fs.reserve(doc.fields_size());
  for (int i = 0; i < doc.fields_size(); i++) {
    auto field = doc.fields(i);
    if (field.has_text_field()) {
      auto tf = field.text_field();
      vector<string> values(tf.values_size());
      for (int j = 0; j < tf.values_size(); j++) {
        values.push_back(tf.values(j));
      }
      fs[i] = make_shared<StringField>(doc.id(), field.field_name(), values);
    } else if (field.has_integer_field()) {
      auto intf = field.integer_field();
      vector<int> values(intf.values_size());
      for (int j = 0; j < intf.values_size(); j++) {
        fs.push_back(make_shared<IntField>(doc.id(), field.field_name(),
                                           intf.values(j)));
      }
    }
  }
  return fs;
}

void DocWrapper::to_stream(ostream *to_stream) {
  doc.SerializeToOstream(to_stream);
}

DocWrapper from_stream(istream *from_stream) {
  Document dt;
  dt.ParseFromIstream(from_stream);
  return dt;
}
