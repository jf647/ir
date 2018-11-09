cc_library(
    name = "doc_iterator",
    srcs = ["doc_iterator.cpp"],
    hdrs = ["doc_iterator.h"],
    deps = [
    ],
)
cc_library(
    name = "index",
    hdrs = ["index.h"],
    srcs = ["index.cpp"],
    deps = [
        "//cpp/ds:skiplist",
        "//cpp/field:field_base",
        "//cpp/query:query",
    ],
)
cc_library(
    name = "string_index",
    srcs = ["string_index.cpp"],
    hdrs = ["string_index.h"],
    deps = [
        ":index",
        ":doc_iterator",
        "//cpp/field:string_fields",
        "//cpp/analyzer:snowball_analyzer",
    ],
)
cc_library(
    name = "index_config",
    hdrs = ["index_config.h"],
    srcs = ["index_config.cpp"],
    deps = [
        ":index",
        ":string_index",
    ],
)
cc_library(
    name = "indexer",
    srcs = ["indexer.cpp"],
    hdrs = ["indexer.h"],
    deps = [
        "//cpp/ds:skiplist",
        "//cpp/field:field_base",
        "//cpp/query:query",
        ":index_config",
    ],
    visibility = ["//visibility:public"],
)

# Tests
cc_test(
    name = "indexer_test",
    srcs = ["indexer_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":indexer",
        "@gtest//:gtest_main",
    ],
)
