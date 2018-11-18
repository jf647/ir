workspace(name = "org_nvr_nvrdb")

http_archive(
    name = "build_stack_rules_proto",
    urls = ["https://github.com/stackb/rules_proto/archive/4c2226458203a9653ae722245cc27e8b07c383f7.tar.gz"],
    sha256 = "0be90d609fcefae9cc5e404540b9b23176fb609c9d62f4f9f68528f66a6839bf",
    strip_prefix = "rules_proto-4c2226458203a9653ae722245cc27e8b07c383f7",
)

http_archive(
     name = "com_google_absl",
     urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
     strip_prefix = "abseil-cpp-master",
)
new_http_archive(
    name = "gtest",
    build_file = "gtest.BUILD",
    sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
    strip_prefix = "googletest-release-1.8.0",
    url = "https://github.com/google/googletest/archive/release-1.8.0.zip",
)
new_http_archive(
    name = "eigen",
    build_file = "eigen.BUILD",
    strip_prefix = "eigen-eigen-b3f3d4950030",
    url = "http://bitbucket.org/eigen/eigen/get/3.3.5.tar.gz",
)
new_http_archive(
    name = "backward",
    build_file = "backward.BUILD",
    #sha256 = "b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0",
    strip_prefix = "backward-cpp-84ae4f5e80381aca765a0810d4c811acae3cd7c7",
    url = "https://github.com/bombela/backward-cpp/archive/84ae4f5e80381aca765a0810d4c811acae3cd7c7.tar.gz",
)

# For Debugging
http_archive(
    name = "co_vsco_bazel_toolchains",
    strip_prefix = "bazel-toolchains-fa39ae4c19b11635b5e7031bed2c989e97e63912",
    url = "https://github.com/vsco/bazel-toolchains/archive/fa39ae4c19b11635b5e7031bed2c989e97e63912.tar.gz",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "2733bb5dddc0b62e794bb761a19e1bf67352bd7e",
    remote = "https://github.com/nelhage/rules_boost.git",
)

load("@build_stack_rules_proto//cpp:deps.bzl", "cpp_proto_compile")
cpp_proto_compile()
load("@build_stack_rules_proto//cpp:deps.bzl", "cpp_grpc_compile")
cpp_grpc_compile()
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

load("@co_vsco_bazel_toolchains//toolchains:repositories.bzl", "bazel_toolchains_repositories")
bazel_toolchains_repositories()

#cpp_proto_repositories()
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

