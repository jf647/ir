cc_library(
    name = "analyzer",
    srcs = ["analyzer.cpp"],
    hdrs = ["analyzer.h"],
    deps = [
    ],
)
cc_library(
    name = "snowball_analyzer",
    srcs = ["snowball_analyzer.cpp"],
    hdrs = ["snowball_analyzer.h"],
    deps = [
        ":analyzer",
        "//cpp/analyzer/snowball:snowball_english",
    ],
    visibility = ["//visibility:public"],
)
cc_test(
    name = "snowball_analyzer_test",
    srcs = ["snowball_analyzer_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":snowball_analyzer",
        "@gtest//:gtest_main",
    ],
)
