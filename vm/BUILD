# file: vm/BUILD
#
# Simple virtual machine build
#

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "vm",
    srcs = [
        "engine.cc",
    ],
    hdrs = [
        "engine.hh",
        "opcodes.hh",
    ],
    deps = [
        "//lib",
    ],
)

cc_binary(
    name = "server",
    srcs = [
        "server.cc",
    ],
    deps = [
        ":vm",
    ],
)
