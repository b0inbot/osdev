# file: drv/BUILD
#
# Drivers and such.
#
load("//:defs.bzl", "nova_lib")

package(default_visibility = ["//visibility:public"])

# UART probably does not belong here but keeping it here for now.
nova_lib(
    name = "uart",
    srcs = ["uart.cc"],
    hdrs = [
        "uart.hh",
    ],
)
