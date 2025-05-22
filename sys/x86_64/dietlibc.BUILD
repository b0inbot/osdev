# file: sys/x86_64/dietlibc.BUILD
#
# BUILD file defining libraries and headers pulled from
# dietlibc
#
load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "headers",
    srcs = [
        "dietlibm.h",
    ] + glob([
        "include/sys/*.h",
        "include/*.h",
    ]),
)

cc_library(
    name = "common",
    hdrs = [
        "dietlibm.h",
    ] + glob([
        "include/sys/*.h",
        "include/*.h",
    ]),
)

cc_library(
    name = "libm",
    srcs = glob(["x86_64/math_*.S"]),
    hdrs = [
        "include/math.h",
    ],
    deps = [":common"],
)
