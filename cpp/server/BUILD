cc_library(
    name = "document_service",
    srcs = ["document_service.cpp"],
    hdrs = ["document_service.h"],
    deps = [
        "//proto:document_service_cpp",
        "//cpp/store:file_store",
    ],
)
cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        ":document_service",
            ],
)
