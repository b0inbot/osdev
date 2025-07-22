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
# The pkg_  and sh_ rules are used to build and run the resulting ISO file that we
# system, respresented as an ISO cdrom.
#
# The layout of the filesystem we want to build looks like this, where
# grub.cfg is the required path and grub.cfg points to the other two files:
#
#  /boot/grub/grub.cfg
#  /boot/x86_64-nova
#  /boot/root
#
# There was no clear way to pass in a pkg_filegroup into geniso so
# the pipeline has a seemingly unnecessary pkg_tar step.
#
# pkg_files(nova) ---------\
# pkg_files(grub.cfg)----> pkg_tar --->  geniso ---> boot.iso
# pkg_files ---------------/
#
# A pkg_iso would be nice except that grub2-rescue does a lot more
# work than just building an ISO.
#

load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_pkg//pkg:mappings.bzl", "pkg_attributes", "pkg_files")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")
load("@rules_shell//shell:sh_binary.bzl", "sh_binary")

exports_files(["boot.cc"])

#
# geniso is a simple script which takes in a tar file, extracts
# it, and runs grub2-mkrescue to generate an ISO file that can be
# booted.
#
sh_binary(
    name = "geniso",
    srcs = ["geniso.sh"],
)

alias(
    name = "root",
    actual = "//hello",
)

#
# Whatever the name of our binary is needs to be put into
# our grub.cfg. annoying... there is probably a way to remap
# the name in pkg_files to something constant.
#
genrule(
    name = "grub-config",
    srcs = ["grub.cfg.in"],
    outs = ["grub.cfg"],
    cmd = "sed 's,$$ROOT,hello,g' < $< > $@",
)

pkg_files(
    name = "grub",
    srcs = [
        ":grub-config",
    ],
    attributes = pkg_attributes(
        group = "root",
        mode = "0551",
        owner = "root",
    ),
    prefix = "boot/grub",
)

#
# We have to keep grub and the rest of the nova files in
# separate pkg_files targets since they have separate prefixes.
#

pkg_files(
    name = "nova",
    srcs = [
        ":root",
        "@nova-x86_64//:kernel",
    ],
    attributes = pkg_attributes(
        group = "root",
        mode = "0551",
        owner = "root",
    ),
    prefix = "boot",
)

pkg_tar(
    name = "nova-all",
    srcs = [
        ":grub",
        ":nova",
    ],
    visibility = ["//visibility:public"],
)

genrule(
    name = "boot",
    srcs = [
        ":nova-all",
    ],
    outs = ["boot.iso"],
    cmd = "$(locations :geniso) $@ $(locations :nova-all)",
    tools = [":geniso"],
    visibility = ["//visibility:public"],
)

sh_binary(
    name = "run",
    srcs = ["run.sh"],
    args = [
        "$(location @ovmf-edk2//:ovmf_code)",
        "$(location @ovmf-edk2//:ovmf_vars)",
        "$(location :boot)",
    ],
    data = [
        ":boot",
        # we need the OVMF files because we want to use UEFI within
        # qemu. We dont need these in the ISO but they need to be
        # passed into QEMU.
        "@ovmf-edk2//:ovmf_code",
        "@ovmf-edk2//:ovmf_vars",
    ],
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
        "//lib:headers",
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
