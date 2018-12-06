workspace(name = "org_nvr_nvrdb")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "build_stack_rules_proto",
    sha256 = "0be90d609fcefae9cc5e404540b9b23176fb609c9d62f4f9f68528f66a6839bf",
    strip_prefix = "rules_proto-4c2226458203a9653ae722245cc27e8b07c383f7",
    urls = ["https://github.com/stackb/rules_proto/archive/4c2226458203a9653ae722245cc27e8b07c383f7.tar.gz"],
)

http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-master",
    urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
)

http_archive(
    name = "gtest",
    build_file = "@//:gtest.BUILD",
    sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
    strip_prefix = "googletest-release-1.8.0",
    urls = ["https://github.com/google/googletest/archive/release-1.8.0.zip"],
)

http_archive(
    name = "eigen",
    build_file = "@//:eigen.BUILD",
    strip_prefix = "eigen-eigen-b3f3d4950030",
    urls = ["http://bitbucket.org/eigen/eigen/get/3.3.5.tar.gz"],
)

http_archive(
    name = "backward",
    build_file = "@//:backward.BUILD",
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

http_archive(
    name = "com_github_nelhage_rules_boost",
    strip_prefix = "rules_boost-2733bb5dddc0b62e794bb761a19e1bf67352bd7e",
    url = "https://github.com/nelhage/rules_boost/archive/2733bb5dddc0b62e794bb761a19e1bf67352bd7e.tar.gz",
)

http_archive(
    name = "io_bazel_rules_python",
    sha256 = "40499c0a9d55f0c5deb245ed24733da805f05aaf6085cb39027ba486faf1d2e1",
    strip_prefix = "rules_python-8b5d0683a7d878b28fffe464779c8a53659fc645",
    url = "https://github.com/bazelbuild/rules_python/archive/8b5d0683a7d878b28fffe464779c8a53659fc645.zip",
)

http_archive(
    name = "mio",
    build_file = "@//:mio.BUILD",
    strip_prefix = "mio-c7c7163a82457d71ecb80e5e3d831dd808117483",
    urls = ["https://github.com/mandreyel/mio/archive/c7c7163a82457d71ecb80e5e3d831dd808117483.tar.gz"],
)

# dependency missing in flatbuffers
http_archive(
    name = "io_bazel_rules_go",
    sha256 = "f87fa87475ea107b3c69196f39c82b7bbf58fe27c62a338684c20ca17d1d8613",
    urls = ["https://github.com/bazelbuild/rules_go/releases/download/0.16.2/rules_go-0.16.2.tar.gz"],
)

http_archive(
    name = "flatbuffers",
    strip_prefix = "flatbuffers-49fed8c4f61f296d1d173a1a8df872a094020278",
    urls = ["https://github.com/google/flatbuffers/archive/49fed8c4f61f296d1d173a1a8df872a094020278.tar.gz"],
)

load("@io_bazel_rules_go//go:def.bzl", "go_register_toolchains", "go_rules_dependencies")

go_rules_dependencies()

go_register_toolchains()

load("@build_stack_rules_proto//cpp:deps.bzl", "cpp_proto_compile")

cpp_proto_compile()

load("@build_stack_rules_proto//cpp:deps.bzl", "cpp_grpc_compile")

cpp_grpc_compile()

load("@build_stack_rules_proto//python:deps.bzl", "python_grpc_library")

python_grpc_library()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@co_vsco_bazel_toolchains//toolchains:repositories.bzl", "bazel_toolchains_repositories")

bazel_toolchains_repositories()

#cpp_proto_repositories()
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

load("@io_bazel_rules_python//python:pip.bzl", "pip_import", "pip_repositories")

pip_repositories()

pip_import(
    name = "protobuf_py_deps",
    requirements = "@build_stack_rules_proto//python/requirements:protobuf.txt",
)

load("@protobuf_py_deps//:requirements.bzl", protobuf_pip_install = "pip_install")

protobuf_pip_install()

pip_import(
    name = "grpc_py_deps",
    requirements = "@build_stack_rules_proto//python:requirements.txt",
)

load("@grpc_py_deps//:requirements.bzl", grpc_pip_install = "pip_install")

grpc_pip_install()

pip_import(
    name = "demo_deps",
    requirements = "//python:requirements.txt",
)

load("@demo_deps//:requirements.bzl", demo_deps_install = "pip_install")

demo_deps_install()
