cc_library(
    name = "index_manager",
    hdrs = ["index_manager.hpp"],
    deps = [
        "//cpp/store:file_store",
        "//cpp/index:indexer"
    ],
)
cc_library(
    name = "search_service",
    hdrs = ["search_service.hpp"],
    deps = [
        "//proto:search_service_cpp",
        ":index_manager",
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
    hdrs = ["admin_service.hpp"],
    deps = [
        "//proto:admin_service_cpp",
        ":index_manager",
    ],
)
cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        ":document_service",
        ":admin_service",
        ":search_service",
    ],
)
