cc_library(
    name = "sl_descriptor",
    hdrs = ["sl_descriptor_generated.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@flatbuffers",
    ],
)

cc_library(
    name = "matrix_descriptor",
    hdrs = ["matrix_descriptor_generated.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":sl_descriptor",
    ],
)

cc_library(
    name = "persistent_sl_gen_fbs",
    hdrs = ["persistent_sl_generated.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@flatbuffers",
    ],
)
