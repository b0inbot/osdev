# file: drv/BUILD
#
# Drivers and such.
#

load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

# UART probably does not belong here but keeping it here for now.
cc_library(
    name = "uart",
    srcs = ["uart.cc"],
    hdrs = [
        "uart.hh",
    ],
    deps = [
        "//lib:headers",
        "//sys:syscalls",
    ],
)
