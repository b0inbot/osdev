# file: sys/nova/BUILD
#
# Build rules for NOVA specific libraries and headers that are
# custom written or imported in directly and modified.
#
load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "hip",
    hdrs = [
        "hip.hh",
    ],
    deps = [
        "//lib:headers",
    ],
)
