# file: sys/nova/BUILD
#
# Build rules for NOVA specific libraries and headers that are
# custom written or imported in directly and modified.
#
load("//:defs.bzl", "nova_lib")

package(default_visibility = ["//visibility:public"])

nova_lib(
    name = "headers",
    hdrs = [
        "ObjectSpace.hh",
        "hip.hh",
    ],
    deps = [
        "//lib",
    ],
)

nova_lib(
    name = "hypercalls",
    srcs = [
        "Hypercalls.cc",
    ],
    hdrs = [
        "Hypercalls.hh",
    ],
    deps = [
        "//lib",
        "//lib:math",
        "//sys:syscalls",
    ],
)
