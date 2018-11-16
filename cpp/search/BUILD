cc_library(
    name = "query_wrapper",
    srcs = ["query_wrapper.cpp"],
    hdrs = ["query_wrapper.h"],
    deps = [
        "//cpp/query:query",
        "//proto:search_models_cpp",
    ],
    visibility = ["//visibility:public"],
)
cc_library(
    name = "searcher",
    srcs = ["searcher.cpp"],
    hdrs = ["searcher.h"],
    deps = [
        "//cpp/index:indexer",
        "//cpp/query:query",
    ],
    visibility = ["//visibility:public"],
)
cc_test(
    name = "searcher_test",
    srcs = ["searcher_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":searcher",
        "@gtest//:gtest_main",
        #"@backward//:stacker",
    ],
)

