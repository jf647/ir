cc_library(
    name="mio",
    hdrs = glob(["include/**"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)

cc_test(
    name = "mio_test",
    srcs = ["test/test.cpp"],
    deps = [
        ":mio",
    ],
)
cc_binary(
    name = "example",
    srcs = ["test/example.cpp"],
    deps = [
        ":mio",
    ],
)

