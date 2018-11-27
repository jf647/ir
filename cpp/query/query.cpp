#include "query.h"

Query::Query() {}
Query::~Query() {}

VectorQuery::VectorQuery(string fieldName, vector<double> _vec, int k)
    : _fieldName(fieldName), _vec(_vec), _k(k) {}
VectorQuery::~VectorQuery() {}
string VectorQuery::fieldName() { return _fieldName; }
vector<double> VectorQuery::query() { return _vec; }
int VectorQuery::k() { return _k; };
void VectorQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }

StringQuery::StringQuery(string fieldName, string query, bool scored)
    : scored(scored), _fieldName(fieldName), _query(query) {}
StringQuery::StringQuery(string fieldName, string query)
    : StringQuery(fieldName, query, false) {}
StringQuery::~StringQuery(){};
string StringQuery::fieldName() { return _fieldName; }
string StringQuery::query() { return _query; }
void StringQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }

IntRangeQuery::IntRangeQuery(string fieldName, int from, int to)
    : _fieldName(fieldName), _from(from), _to(to) {}
IntRangeQuery::~IntRangeQuery(){};
string IntRangeQuery::fieldName() { return _fieldName; }
int IntRangeQuery::from() { return _from; }
int IntRangeQuery::to() { return _to; };
void IntRangeQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }

NestedQuery::NestedQuery(vector<shared_ptr<Query>> queries, Combiner combiner)
    : _queries(queries), _combiner(combiner) {}
vector<shared_ptr<Query>> NestedQuery::queries() { return _queries; }
Combiner NestedQuery::combiner() { return _combiner; }
void NestedQuery::accept(QueryVisitor &visitor) { visitor.visit(this); }
