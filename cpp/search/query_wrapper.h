#ifndef QUERY_WRAPPER_H
#define QUERY_WRAPPER_H
#include "cpp/query/query.h"
#include "proto/search/search.pb.h"
using search::SearchRequest;
using std::make_shared;

class QueryWrapper {
private:
  SearchRequest _req;
  shared_ptr<Query> query(const search::Params &params, int k) {
    vector<shared_ptr<Query>> qs;
    qs.reserve(params.params_size());
    for (int i = 0; i < _req.params().params_size(); i++) {
      auto q = _req.params().params(i);
      if (q.has_query_param()) {
        auto qp = q.query_param();
        for (int j = 0; j < qp.fields_size(); j++)
          qs.push_back(make_shared<StringQuery>(qp.fields(j), qp.query()));
      } else if (q.has_filter_param()) {
        auto fp = q.filter_param();
        qs.push_back(make_shared<StringQuery>(fp.field(), fp.string_filter()));
      } else if (q.has_range_param()) {
        auto rp = q.range_param();
        if (rp.bound().has_int_bound()) {
          auto ip = rp.bound().int_bound();
          qs.push_back(make_shared<IntRangeQuery>(rp.field(), ip.lower_bound(),
                                                  ip.upper_bound()));
        }
      } else if (q.has_vector_param()) {
        auto vp = q.vector_param();
        vector<double> vq(vp.vector_field_size());
        for (int j = 0; j < vp.vector_field_size(); j++) {
          vq[j] = vp.vector_field(j);
        }
        qs.push_back(make_shared<VectorQuery>(vp.field(), vq, k));
      } else if (q.has_composed_param()) {
        qs.push_back(query(q.composed_param(), k));
      }
    }
    switch (params.combine_with()) {
    case search::COMBINE_WITH_SHOULD:
      return make_shared<NestedQuery>(qs, SHOULD);
    case search::COMBINE_WITH_MUST:
    default:
      return make_shared<NestedQuery>(qs, MUST);
    }
  }

public:
  QueryWrapper(const SearchRequest &req) { _req.CopyFrom(req); }
  shared_ptr<Query> query() { return query(_req.params(), _req.count()); }
  virtual ~QueryWrapper() {}
};

#endif /* QUERY_WRAPPER_H */
