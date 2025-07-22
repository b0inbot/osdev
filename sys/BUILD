# file: sys/BUILD
#
# Build rules and libraries which are facades for their architecture
# specific counterparts.
#

load("//:defs.bzl", "nova_lib")

package(
    default_visibility = ["//visibility:public"],
)

nova_lib(
    name = "syscalls",
    hdrs = [
        "syscalls.hh",
    ],
    deps = select({
        "@platforms//cpu:x86_64": ["//sys/x86_64:syscalls"],
    }),
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

#
# 'boot.cc' is shared boot code in each rootpd. it's injected into the rootpd
# build via the macro defined under //:defs.bzl
#
alias(
    name = "boot.cc",
    actual = select({
        "@platforms//cpu:x86_64": "//sys/x86_64:boot.cc",
    }),
)
