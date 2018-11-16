#include "admin_service.hpp"
#include "document_service.hpp"
#include "search_service.hpp"

//#include "glog/logging.h"

using grpc::InsecureServerCredentials;
using std::make_shared;

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  Config fileStoreConfig;
  shared_ptr<FileStore> store = make_shared<FileStore>(fileStoreConfig);
  shared_ptr<IndexManager> manager = make_shared<IndexManager>(store);

  try {
    DocumentServiceImpl service(store);
    admin::AdminServiceImpl adminService(manager);
    SearchServiceImpl searchService(manager);
    ServerBuilder builder;
    builder.AddListeningPort(server_address, InsecureServerCredentials());
    builder.RegisterService(&service);
    builder.RegisterService(&adminService);
    builder.RegisterService(&searchService);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on " << server_address << std::endl;
    server->Wait();
  } catch (const char *msg) {
    std::cerr << msg << std::endl;
  }
}
int main(int argc, char **argv) {
  // google::InitGoogleLogging(argv[0]);
  RunServer();
  return 0;
}
