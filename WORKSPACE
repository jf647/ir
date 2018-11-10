workspace(name = "org_nvr_nvrdb")

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
