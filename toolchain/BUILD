# file: toolchain/BUILD
#
# x86-64 cross compiler designed for some level (not fully) hermeticity.
#
# The actual cross-compiler is not included but is based on the osdev wiki guide.
# https://wiki.osdev.org/GCC_Cross-Compiler#Preparing_for_the_build
# I ignored compiling and installing libstdc++-v3 for now.
#
# There is more to do here, move linker script over so our cc_* bazel rule invocations
# do not need any new flags.
#
load("@rules_cc//cc/toolchains:cc_toolchain.bzl", "cc_toolchain")
load(":cc_toolchain_config.bzl", "cc_toolchain_config")

filegroup(name = "empty")

cc_toolchain_config(
    name = "nova_x86_64_toolchain_config",
    dietlibc = "@dietlibc-x86_64//:headers",
)

filegroup(
    name = "includes",
    srcs = [
        "@dietlibc-x86_64//:headers",
    ],
)

cc_toolchain(
    name = "nova_x86_64_cc_toolchain",
    all_files = ":includes",
    compiler_files = ":includes",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":nova_x86_64_toolchain_config",
)

toolchain(
    name = "cc_nova_x86_64_cc_toolchain",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
        # TODO: This breaks genrules
        # "//:nova-boin",
    ],
    toolchain = ":nova_x86_64_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)
