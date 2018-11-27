package(default_visibility = ["//visibility:public"])
cc_library(
    name = "config",
    hdrs = ["config.h"],
    deps = [
    ],
)
cc_library(
    name = "file_store",
    hdrs = ["file_store.h"],
    deps = [
        ":config",
        ":doc_wrapper",
        "//cpp/index:indexer",
        "//cpp/utils:logger",
    ],
)
cc_library(
    name = "doc_wrapper",
    srcs = ["doc_wrapper.cpp"],
    hdrs = ["doc_wrapper.h"],
    deps = [
        "//proto:document_models_cpp",
        "//cpp/field:fields",
    ],
)
