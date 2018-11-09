cc_library(
    name = "skiplist",
    srcs = ["skiplist.cpp"],
    hdrs = ["skiplist.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "skiplist_test",
    srcs = ["skiplist_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":skiplist",
        "@gtest//:gtest_main",
    ],
)
