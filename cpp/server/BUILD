cc_library(
    name = "index_manager",
    hdrs = ["index_manager.h"],
    srcs = ["index_manager.cpp"],
    deps = [
        "//cpp/store:file_store",
        "//cpp/index:indexer",
        "//cpp/utils:logger",
    ],
)
cc_library(
    name = "search_service",
    hdrs = ["search_service.hpp"],
    srcs = [
        "index_manager.h",
    ],
    deps = [
        "//proto:search_service_cpp",
        "//cpp/search:searcher",
        "//cpp/search:query_wrapper",
    ],
)
cc_library(
    name = "document_service",
    hdrs = ["document_service.hpp"],
    deps = [
        "//proto:document_service_cpp",
        "//cpp/store:file_store",
    ],
)
cc_library(
    name = "admin_service",
    hdrs = ["admin_service.h"],
    srcs = [
        "admin_service.cpp",
        ],
    deps = [
        "//proto:admin_service_cpp",
        ":index_manager",
        "//cpp/utils:logger",
    ],
)
cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        ":document_service",
        ":admin_service",
        ":search_service",
        "//cpp/utils:init_logger",
        "//cpp/utils:stacktrace",
        #"@backward//:stacker",
    ],
)
cc_binary(
    name = "play",
    srcs = ["play.cpp"],
    deps = [
    ],
)

