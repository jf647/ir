cc_library(
    name = "persistent_matrix",
    srcs = ["persistent_matrix.cpp"],
    hdrs = ["persistent_matrix.h"],
    deps = [
        "//flatb:matrix_descriptor",
        "@eigen",
        "@mio",
    ],
)

cc_test(
    name = "persistent_matrix_test",
    srcs = ["persistent_matrix_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":persistent_matrix",
        "@backward//:stacker",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "sl",
    srcs = ["sl.cpp"],
    hdrs = ["sl.h"],
    deps = [
    ],
)

cc_library(
    name = "pslv2",
    srcs = ["pslv2.cpp"],
    hdrs = ["pslv2.h"],
    deps = [
        ":sl",
        "//flatb:persistent_sl_gen_fbs",
        "//flatb:sl_descriptor",
    ],
)

cc_library(
    name = "persistent_sl",
    srcs = ["persistent_sl.cpp"],
    hdrs = ["persistent_sl.h"],
    deps = [
        ":sl",
        "//cpp/utils:logger",
        "//flatb:persistent_sl_gen_fbs",
        "@mio",
    ],
)

cc_library(
    name = "psl_string",
    srcs = ["psl_string.cpp"],
    hdrs = ["psl_string.h"],
    deps = [
        ":persistent_sl",
    ],
)

cc_test(
    name = "persistent_sl_test",
    srcs = ["persistent_sl_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":psl_string",
        "@backward//:stacker",
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
