package(default_visibility = ["//visibility:public"])

cc_library(
    name = "eigen_nth",
    hdrs = ["eigen_nth.h"],
    srcs = ["eigen_nth.cpp"],
    deps = [
        "@eigen",
    ],
)

cc_test(
    name = "eigen_nth_test",
    srcs = ["eigen_nth_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":eigen_nth",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "eigen_mask",
    hdrs = ["eigen_mask.h"],
    srcs = ["eigen_mask.cpp"],
    deps = [
        "@eigen",
    ],
)

cc_test(
    name = "eigen_mask_test",
    srcs = ["eigen_mask_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":eigen_mask",
        "@gtest//:gtest_main",
    ],
)
