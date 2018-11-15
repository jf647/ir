#ifndef DOCUMENT_SERVICE_H
#define DOCUMENT_SERVICE_H

#include "proto/document.grpc.pb.h"
#include "proto/document.pb.h"
#include <grpc++/grpc++.h>

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

class DocumentServiceImpl final : public DocumentService::Service {
public:
  explicit DocumentServiceImpl();
  Status Put(ServerContext *context, const IndexRequest *request,
             IndexResponse *response) override;
  Status Get(ServerContext *context, const GetDocumentRequest *request,
             GetDocumentResponse *response) override;
};

#endif /* DOCUMENT_SERVICE_H */
