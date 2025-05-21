# file: sys/nova/BUILD
#
# Build rules for NOVA specific libraries and headers that are
# custom written or imported in directly and modified.
#
load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "headers",
    hdrs = [
        "ObjectSpace.hh",
        "hip.hh",
    ],
    deps = [
        "//lib:headers",
    ],
)

cc_library(
    name = "hypercalls",
    srcs = [
        "Hypercalls.cc",
    ],
    hdrs = [
        "Hypercalls.hh",
    ],
    deps = [
        "//lib:headers",
        "//lib:math",
        "//sys:nothing",
        "//sys:syscalls",
    ],
)
