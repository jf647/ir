#ifndef DOCUMENT_SERVICE_H
#define DOCUMENT_SERVICE_H

#include "cpp/store/file_store.h"
#include "proto/document/document.grpc.pb.h"
#include "proto/document/document.pb.h"
#include <grpc++/grpc++.h>
#include <memory>

using document::Document;
using document::DocumentService;
using document::GetDocumentRequest;
using document::GetDocumentResponse;
using document::IndexRequest;
using document::IndexResponse;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using std::shared_ptr;

class DocumentServiceImpl final : public DocumentService::Service {
  shared_ptr<FileStore> store;

public:
  explicit DocumentServiceImpl(shared_ptr<FileStore> store);
  Status Put(ServerContext *context, const IndexRequest *request,
             IndexResponse *response) override;
  Status Get(ServerContext *context, const GetDocumentRequest *request,
             GetDocumentResponse *response) override;
};

DocumentServiceImpl::DocumentServiceImpl(shared_ptr<FileStore> store)
    : store(store) {}

Status DocumentServiceImpl::Put(ServerContext *context,
                                const IndexRequest *request,
                                IndexResponse *response) {
  Document d = request->document();
  store->store(d);
  std::string prefix("Put ");
  response->set_response_code(prefix + d.id());
  return Status::OK;
}

Status DocumentServiceImpl::Get(ServerContext *context,
                                const GetDocumentRequest *request,
                                GetDocumentResponse *response) {
  std::string prefix("Get ");
  response->set_response_code(prefix + request->id());
  auto dw = store->fetch(request->id());
  response->mutable_document()->CopyFrom(dw.doc);
  return Status::OK;
}

#endif /* DOCUMENT_SERVICE_H */
