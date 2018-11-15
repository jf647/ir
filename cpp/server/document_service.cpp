#include "document_service.h"

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

DocumentServiceImpl::DocumentServiceImpl() {}

Status DocumentServiceImpl::Put(ServerContext *context,
                                const IndexRequest *request,
                                IndexResponse *response) {
  Document d = request->document();
  std::string prefix("Put ");
  response->set_response_code(prefix + d.id());
  return Status::OK;
}

Status DocumentServiceImpl::Get(ServerContext *context,
                                const GetDocumentRequest *request,
                                GetDocumentResponse *response) {
  std::string prefix("Get ");
  response->set_response_code(prefix + request->id());
  return Status::OK;
}
