load("@demo_deps//:requirements.bzl", "requirement")
load("@build_stack_rules_proto//python:python_grpc_library.bzl", "python_grpc_library")

python_grpc_library(
  name = "protos_py",
  deps = [
      "//proto/document:document_proto",
      "//proto/admin:admin_proto",
      "//proto/search:search_proto",
  ],
)
py_binary(
    name = "demo",
    srcs = [
        "demo.py",
    ],
    deps = [
        ":protos_py",
        requirement("grpcio"),
        requirement("protobuf"),
        requirement("click"),
    ],
)
