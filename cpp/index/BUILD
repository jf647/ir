cc_library(
    name = "indexer",
    srcs = ["indexer.cpp"],
    hdrs = ["indexer.h"],
    deps = [
        "//cpp/ds:skiplist",
        "//cpp/field:int_field",
    ],
)

cc_test(
    name = "indexer_test",
    srcs = ["indexer_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":indexer",
        "@gtest//:main",
    ],
)
