#ifndef QUERY_H
#define QUERY_H

class Query {
private:
protected:
  Query();
  virtual ~Query();
};
class IntRangeQuery : public Query {
public:
  IntRangeQuery(int from, to) : _from(from), _to(to) {}
  virtual ~IntRangeQuery(){};

private:
  int _from;
  int _to;
};
#endif /* QUERY_H */
