#ifndef QUERY_H
#define QUERY_H
#include "cpp/field/int_field.h"

class Query {
private:
protected:
  Query(){};
  virtual ~Query(){};
};
class IntRangeQuery : public Query {
public:
  IntRangeQuery(int from, int to) : _from(from), _to(to) {}
  virtual ~IntRangeQuery(){};
  IntField from() { return IntField(_from); }
  IntField to() { return IntField(_to); };

private:
  int _from;
  int _to;
};
#endif /* QUERY_H */
