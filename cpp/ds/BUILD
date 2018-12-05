cc_library(
    name = "sl",
    srcs = ["sl.cpp"],
    hdrs = ["sl.h"],
    deps = [
    ],
)
cc_library(
    name = "persistent_sl",
    srcs = ["persistent_sl.cpp"],
    hdrs = ["persistent_sl.h"],
    deps = [
        ":sl",
        "@mio//:mio",
        "//flatb:persistent_sl_gen_fbs",
        "//cpp/utils:logger",
        ":psl_string",
    ],
)
cc_library(
    name = "psl_string",
    srcs = ["psl_string.cpp"],
    hdrs = ["psl_string.h"],
    deps = [
    ],
)
cc_test(
    name = "persistent_sl_test",
    srcs = ["persistent_sl_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":persistent_sl",
        "@gtest//:gtest_main",
    ],
)
cc_test(
    name = "sl_test",
    srcs = ["sl_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":sl",
        "@gtest//:gtest_main",
    ],
)
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
