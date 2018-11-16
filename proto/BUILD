package(default_visibility = ["//visibility:public"])
#load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")
#
#cc_proto_library(
#    name = "document_service_cpp",
#    protos = ["document.proto"],
#    with_grpc = True,
#)

load("@build_stack_rules_proto//cpp:cpp_proto_library.bzl", "cpp_proto_library")
load("@build_stack_rules_proto//cpp:cpp_grpc_library.bzl", "cpp_grpc_library")

cpp_proto_library(
    name = "document_models_cpp",
    deps = ["//proto/document:document_proto"],
)
cpp_grpc_library(
    name = "document_service_cpp",
    deps = ["//proto/document:document_proto"],
)

cpp_grpc_library(
    name = "admin_service_cpp",
    deps = ["//proto/admin:admin_proto"],
)

cpp_proto_library(
    name = "search_models_cpp",
    deps = ["//proto/search:search_proto"],
)
cpp_grpc_library(
    name = "search_service_cpp",
    deps = ["//proto/search:search_proto"],
)
