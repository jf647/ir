package(default_visibility = ["//visibility:public"])
cc_library(
    name = "vector_field",
    srcs = ["vector_field.cpp"],
    hdrs = ["vector_field.h"],
    deps = [
        ":field_base",
    ],
)
cc_library(
    name = "string_fields",
    srcs = ["string_fields.cpp"],
    hdrs = ["string_fields.h"],
    deps = [
        ":field_base",
    ],
)
cc_library(
    name = "field_base",
    srcs = ["field_base.cpp"],
    hdrs = ["field_base.h"],
)
cc_library(
    name = "fields",
    hdrs = ["fields.h"],
    deps = [
        ":string_fields",
        ":vector_field",
    ],
)
