#ifndef QUERY_H
#define QUERY_H

#include <memory>
#include <string>
#include <vector>

using std::string;

using std::shared_ptr;
using std::vector;

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
  int from() { return _from; }
  int to() { return _to; };
  void accept(QueryVisitor &visitor);

private:
  string _fieldName;
  int _from;
  int _to;
};
class StringQuery : public Query {
public:
  bool scored;
  StringQuery(string fieldName, string query, bool scored)
      : scored(scored), _fieldName(fieldName), _query(query) {}
  StringQuery(string fieldName, string query)
      : StringQuery(fieldName, query, false) {}
  virtual ~StringQuery(){};
  string fieldName() { return _fieldName; }
  string query() { return _query; }
  void accept(QueryVisitor &visitor);

private:
  string _fieldName;
  string _query;
};
class VectorQuery : public Query {
private:
  string _fieldName;
  vector<double> _vec;
  int _k;

public:
  explicit VectorQuery(string fieldName, vector<double> _vec, int k)
      : _fieldName(fieldName), _vec(_vec), _k(k) {}
  virtual ~VectorQuery() {}
  string fieldName() { return _fieldName; }
  vector<double> query() { return _vec; }
  int k() { return _k; };
  void accept(QueryVisitor &visitor);
};
class QueryVisitor {
public:
  virtual void visit(IntRangeQuery *q) = 0;
  virtual void visit(NestedQuery *q) = 0;
  virtual void visit(StringQuery *q) = 0;
  virtual void visit(VectorQuery *q) = 0;
};

void IntRangeQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }
void StringQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }
void NestedQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }
void VectorQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }

#endif /* QUERY_H */
