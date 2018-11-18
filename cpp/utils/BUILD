package(default_visibility = ["//visibility:public"])
cc_library(
    name = "logger",
    hdrs = ["logger.hpp"],
    deps = [
        "@boost//:log",
    ],
)
cc_library(
    name = "init_logger",
    hdrs = ["init_logger.hpp"],
    deps = [
        ":logger",
    ],
)

cc_library(
    name = "stacktrace",
    hdrs = ["stacktrace.hpp"],
    deps = [
        "@boost//:stacktrace",
        ":logger",
    ],
)
