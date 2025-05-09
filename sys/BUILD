# file: sys/BUILD
#
# Build rules and libraries which are facades for their architecture
# specific counterparts.
#

load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "syscalls",
    hdrs = [
        "syscalls.hh",
    ],
)

#
# 'nothing' represents the cpp and linker flags which strip out
# the stdlib and such.
#
alias(
    name = "nothing",
    actual = select({
        "@platforms//cpu:x86_64": "//sys/x86_64:nothing",
    }),
)

#
# 'rootpd' represents the cpp and linker flags specific to the first
# userspace program that NOVA delegates to.
#
alias(
    name = "rootpd",
    actual = select({
        "@platforms//cpu:x86_64": "//sys/x86_64:rootpd",
    }),
)
