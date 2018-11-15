package(default_visibility = ["//visibility:public"])
load("@build_stack_rules_proto//cpp:cpp_grpc_library.bzl", "cpp_grpc_library")

proto_library(
    name = "document_proto",
    srcs = ["document.proto"],
    deps = [
    ],
)

cpp_grpc_library(
  name = "document_service_cpp",
  deps = [":document_proto"],
)
