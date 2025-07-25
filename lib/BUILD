# file: lib/BUILD
#
# Common/shared libraries and headers
#
load("//:defs.bzl", "nova_lib", "shared_lib")

package(default_visibility = ["//visibility:public"])

shared_lib(
    name = "lib",
    srcs = [
        "IO.cc",
    ],
    hdrs = [
        "IO.hh",
        "System.hh",
        "debug.hh",
        "types.hh",
    ],
)

nova_lib(
    name = "math",
    hdrs = [
        "math.hh",
    ],
    deps = [
        "@dietlibc-x86_64//:libm",
    ],
)
