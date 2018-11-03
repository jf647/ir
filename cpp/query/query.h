#ifndef QUERY_H
#define QUERY_H
#include "cpp/field/int_field.h"

class QueryVisitor;
class Query {
private:
protected:
  Query(){};

public:
  virtual ~Query(){};
  virtual void accept(QueryVisitor &visitor) = 0;
};
/*! \enum Combiners
 *
 *  Detailed description
 */
enum Combiner { MUST, SHOULD };
class NestedQuery : public Query {
private:
  vector<shared_ptr<Query>> _queries;
  Combiner _combiner;

public:
  NestedQuery(vector<shared_ptr<Query>> queries, Combiner combiner)
      : _queries(queries), _combiner(combiner) {}
  vector<shared_ptr<Query>> queries() { return _queries; }
  Combiner combiner() { return _combiner; }
  void accept(QueryVisitor &visitor);
};
class IntRangeQuery : public Query {
public:
  IntRangeQuery(string fieldName, int from, int to)
      : _fieldName(fieldName), _from(from), _to(to) {}
  virtual ~IntRangeQuery(){};
  string fieldName() { return _fieldName; }
  IntField from() { return IntField(_from); }
  IntField to() { return IntField(_to); };
  void accept(QueryVisitor &visitor);

private:
  string _fieldName;
  int _from;
  int _to;
};
class QueryVisitor {
public:
  virtual void visit(IntRangeQuery *q) = 0;
  virtual void visit(NestedQuery *q) = 0;
};

void IntRangeQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }
void NestedQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }

#endif /* QUERY_H */
