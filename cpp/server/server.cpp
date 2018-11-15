#include "document_service.h"

void RunServer() {
  std::string server_address("0.0.0.0:50051");

  try {
    DocumentServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
  } catch (const char *msg) {
    std::cerr << msg << std::endl;
  }
}
int main(int argc, char **argv) {
  RunServer();
  return 0;
}
