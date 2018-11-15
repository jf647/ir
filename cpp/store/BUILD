package(default_visibility = ["//visibility:public"])
cc_library(
    name = "config",
    srcs = ["config.cpp"],
    hdrs = ["config.h"],
    deps = [
    ],
)
cc_library(
    name = "file_store",
    srcs = ["file_store.cpp"],
    hdrs = ["file_store.h"],
    deps = [
        ":config",
        ":doc_wrapper",
    ],
)
cc_library(
    name = "doc_wrapper",
    srcs = ["doc_wrapper.cpp"],
    hdrs = ["doc_wrapper.h"],
    deps = [
        "//proto:document_service_cpp",
        "//cpp/field:fields",
    ],
)
