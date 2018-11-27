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
  Query();

public:
  virtual ~Query();
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
  NestedQuery(vector<shared_ptr<Query>> queries, Combiner combiner);
  vector<shared_ptr<Query>> queries();
  Combiner combiner();
  void accept(QueryVisitor &visitor);
};

class IntRangeQuery : public Query {
public:
  IntRangeQuery(string fieldName, int from, int to);
  virtual ~IntRangeQuery();
  string fieldName();
  int from();
  int to();
  void accept(QueryVisitor &visitor);

private:
  string _fieldName;
  int _from;
  int _to;
};

class StringQuery : public Query {
public:
  bool scored;
  StringQuery(string fieldName, string query, bool scored);
  StringQuery(string fieldName, string query);
  virtual ~StringQuery();
  string fieldName();
  string query();
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
  explicit VectorQuery(string fieldName, vector<double> _vec, int k);
  virtual ~VectorQuery();
  string fieldName();
  vector<double> query();
  int k();
  void accept(QueryVisitor &visitor);
};
class QueryVisitor {
public:
  virtual void visit(IntRangeQuery *q) = 0;
  virtual void visit(NestedQuery *q) = 0;
  virtual void visit(StringQuery *q) = 0;
  virtual void visit(VectorQuery *q) = 0;
};

#endif /* QUERY_H */
