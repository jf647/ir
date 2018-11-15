cc_library(
    name = "searcher",
    srcs = ["searcher.cpp"],
    hdrs = ["searcher.h"],
    deps = [
        "//cpp/index:indexer",
        "//cpp/query:query",
    ],
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

