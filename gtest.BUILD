#cc_library(
#    name = "main",
#    srcs = glob(
#        ["src/*.cc"],
#        exclude = ["src/gtest-all.cc"]
#    ),
#    hdrs = glob([
#        "include/**/*.h",
#        "src/*.h"
#    ]),
#    copts = ["-Iexternal/gtest/include"],
#    linkopts = ["-pthread"],
#    visibility = ["//visibility:public"],
#
package(default_visibility = ["//visibility:public"])

licenses(["notice"])

config_setting(
    name = "windows",
    values = {"cpu": "x64_windows"},
)

config_setting(
    name = "windows_msvc",
    values = {"cpu": "x64_windows_msvc"},
)

config_setting(
    name = "has_absl",
    values = {"define": "absl=1"},
)

# Google Test including Google Mock
cc_library(
    name = "gtest",
    srcs = glob(
        include = [
            "googletest/src/*.cc",
            "googletest/src/*.h",
            "googletest/include/gtest/**/*.h",
            "googlemock/src/*.cc",
            "googlemock/include/gmock/**/*.h",
        ],
        exclude = [
            "googletest/src/gtest-all.cc",
            "googletest/src/gtest_main.cc",
            "googlemock/src/gmock-all.cc",
            "googlemock/src/gmock_main.cc",
        ],
    ),
    hdrs = glob([
        "googletest/include/gtest/*.h",
        "googlemock/include/gmock/*.h",
    ]),
    copts = select(
        {
            ":windows": [],
            ":windows_msvc": [],
            "//conditions:default": ["-pthread"],
        },
    ),
    defines = select(
        {
            ":has_absl": [
                "GTEST_HAS_ABSL=1",
            ],
            "//conditions:default": [],
        },
    ),
    includes = [
        "googlemock",
        "googlemock/include",
        "googletest",
        "googletest/include",
    ],
    linkopts = select({
        ":windows": [],
        ":windows_msvc": [],
        "//conditions:default": [
            "-pthread",
        ],
    }),
    deps = select(
        {
            ":has_absl": [
                "@com_google_absl//absl/debugging:failure_signal_handler",
                "@com_google_absl//absl/debugging:stacktrace",
                "@com_google_absl//absl/debugging:symbolize",
                "@com_google_absl//absl/strings",
                "@com_google_absl//absl/types:optional",
                "@com_google_absl//absl/types:variant",
            ],
            "//conditions:default": [],
        },
    ),
)

cc_library(
    name = "gtest_main",
    srcs = [
        "googlemock/src/gmock_main.cc",
    ],
    deps = [":gtest"],
)
