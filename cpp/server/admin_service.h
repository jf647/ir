#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H
#include "proto/admin/admin.grpc.pb.h"
#include "proto/admin/admin.pb.h"
#include <grpc++/grpc++.h>

#include "cpp/index/indexer.h"
#include "cpp/utils/logger.hpp"
#include "index_manager.h"
namespace admin {

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class AdminServiceImpl final : public AdminService::Service {
  shared_ptr<IndexManager> indexManager;

public:
  explicit AdminServiceImpl(shared_ptr<IndexManager> manager);
  Status CreateIndex(ServerContext *context, const CreateIndexRequest *request,
                     CreateIndexResponse *response) override;
};
AdminServiceImpl::AdminServiceImpl(shared_ptr<IndexManager> manager)
    : indexManager(manager) {}
Status AdminServiceImpl::CreateIndex(ServerContext *context,
                                     const CreateIndexRequest *request,
                                     CreateIndexResponse *response) {
  auto schema = request->index_schema();
  auto index = indexManager->Get(schema.index_name());
  if (index) {
    BOOST_LOG_TRIVIAL(info) << "Index already exists: " << schema.index_name();
    return Status::OK;
  }
  vector<shared_ptr<indexer::FieldConfig>> fieldConfigs;
  fieldConfigs.reserve(schema.field_configs_size());
  for (int i = 0; i < schema.field_configs_size(); ++i) {
    auto fieldSchema = schema.field_configs(i);
    switch (fieldSchema.field_type()) {
    case FIELD_TYPE_STRING_ANALYZED:
      fieldConfigs.push_back(make_shared<StringFieldConfig>(
          fieldSchema.field_name(), SNOWBALL, true));
      break;
    case FIELD_TYPE_STRING_NON_ANALYZED:
      fieldConfigs.push_back(make_shared<StringFieldConfig>(
          fieldSchema.field_name(), NONE, false));
      break;
    case FIELD_TYPE_INT:
      fieldConfigs.push_back(
          make_shared<IntFieldConfig>(fieldSchema.field_name()));
      break;
    case FIELD_TYPE_VECTOR:
      fieldConfigs.push_back(
          make_shared<VectorFieldConfig>(fieldSchema.field_name(), 3));
      break;
    case FIELD_TYPE_DOUBLE:
    default:
      break;
    }
  }
  fieldConfigs.shrink_to_fit();
  IndexConfig indexConfig(fieldConfigs);
  indexManager->Register(schema.index_name(),
                         make_shared<Indexer>(indexConfig));
  BOOST_LOG_TRIVIAL(info) << "Index created: " << schema.index_name();
  return Status::OK;
}
} // namespace admin

#endif /* ADMIN_SERVICE_H */
