#ifndef SEARCH_SERVICE_H
#define SEARCH_SERVICE_H
#include "cpp/search/query_wrapper.h"
#include "cpp/search/searcher.h"
#include "index_manager.hpp"
#include "proto/search/search.grpc.pb.h"
#include "proto/search/search.pb.h"
#include <memory>
using search::SearchService;
using std::shared_ptr;

class SearchServiceImpl final : public SearchService::Service {
  shared_ptr<IndexManager> manager;

public:
  explicit SearchServiceImpl(shared_ptr<IndexManager> manager)
      : manager(manager) {}
  grpc::Status Query(grpc::ServerContext *context,
                     const search::SearchRequest *request,
                     search::SearchResponse *response) override {
    auto ir = manager->Get(request->index_name());
    if (!ir) {
      return Status(grpc::NOT_FOUND, "index not dound");
    }
    auto indexReader = *ir;
    Searcher s(indexReader);
    QueryWrapper qw(*request);
    auto query = qw.query();
    if (!query)
      return Status(grpc::INTERNAL, "query is null");

    for (string res : s.query(query)) {
      cout << res << endl;
    }
    cout << "fin quering" << endl;

    return Status::OK;
  }
};

#endif /* SEARCH_SERVICE_H */
