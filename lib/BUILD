# file: lib/BUILD
#
# Common/shared libraries and headers
#

load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "headers",
    hdrs = [
        "debug.hh",
        "types.hh",
    ],
)

cc_library(
    name = "math",
    hdrs = [
        "math.hh",
    ],
    deps = [
        "@dietlibc-x86_64//:libm",
    ],
)
