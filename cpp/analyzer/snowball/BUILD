cc_library(
    name = "header",
    hdrs = ["header.h"],
)
cc_library(
    name = "api",
    srcs = ["api.c","utilities.c"],
    hdrs = ["api.h"],
    deps = [
        ":header",
    ],
)
cc_library(
    name = "snowball_english",
    srcs = [
        "snowball_english.c",
    ],
    hdrs = ["snowball_english.h"],
    deps = [
        ":api",
    ],
    visibility = ["//visibility:public"],
)
