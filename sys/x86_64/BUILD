# file: sys/x86_64/BUILD
#
# Bazel build file which defines the x84_64 build and test run
# scripts.
#
# entry is both the beginning of the first user space program
# and the two syscall methods (named _syscall/_syscall2)
#
# syscalls is the package for the C/C++ callable syscall methods.
# they map into the NOVA hypercalls and delegate to the more generic
# syscall assembly methods.
#
# We use specific linker flags, CXXFLAGS, and linker script in
# order to prepare the binary to run on NOVA. This means
# each cc_library should depend on //sys:nothing and each
# cc_binary should depend on //sys:binary or //sys:rootpd.
#

load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_shell//shell:sh_binary.bzl", "sh_binary")

exports_files([
    "boot.cc",
    "grub.cfg.in",
])

#
# geniso is a simple script which takes in a tar file, extracts
# it, and runs grub2-mkrescue to generate an ISO file that can be
# booted.
#
sh_binary(
    name = "geniso",
    srcs = ["geniso.sh"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "entry",
    srcs = [
        "entry.S",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":nothing",
    ],
)

cc_library(
    name = "syscalls",
    srcs = [
        "syscalls.cc",
    ],
    hdrs = [
        "syscalls.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":entry",
        "//lib",
    ],
)

genrule(
    name = "rootpd.ld",
    srcs = ["rootpd.in.ld"],
    outs = ["rootpd.out.ld"],
    cmd = "cp $< $@",
    target_compatible_with = [
        "@platforms//cpu:x86_64",
    ],
)

cc_library(
    name = "nothing",
    copts =
        [
            "-std=c++20",
            "-fno-builtin",
            "-nostartfiles",
            "-nolibc",
            "-nostdlib",
            "-DX86_64",
            "-fomit-frame-pointer",
            "-fno-exceptions",
            "-fno-asynchronous-unwind-tables",
            "-fno-unwind-tables",
        ],
    defines = [
        "X86_64",
    ],
    linkopts =
        [
            "-fno-builtin",
            "-nostartfiles",
            "-nolibc",
            "-nostdlib",
        ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rootpd",
    copts = [
        "-DROOTPD",
    ],
    defines = [
        "X86_64",
        "ROOTPD",
    ],
    linkopts =
        [
            "-T$(location :rootpd.ld)",
            "-Wl,--build-id=none",
            "-fomit-frame-pointer",
            "-fno-exceptions",
            "-fno-asynchronous-unwind-tables",
            "-fno-unwind-tables",
        ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":entry",
        ":nothing",
        ":rootpd.ld",
        ":syscalls",
        "//sys:syscalls",
    ],
)
