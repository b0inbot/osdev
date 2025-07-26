"""
Macros to wrap our usage of cc_* rules so that we can
tweak various options project-wide.
"""

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_pkg//pkg:mappings.bzl", "pkg_attributes", "pkg_files")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")
load("@rules_shell//shell:sh_binary.bzl", "sh_binary")

# the rootpd is the main userspace entrypoint. It
# requires a _main which is in //sys:boot.cc and
# rootpd expects a pmain function definition.
#
def _nova_rootpd_impl(name, srcs, deps, target_compatible_with, **kwargs):
    ddeps = (deps or []) + [
        "//drv:uart",
        "//lib",
        "//lib:math",
        "//sys:rootpd",
        "//sys/nova:headers",
        "//sys/nova:hypercalls",
    ]
    target_compatible_with = target_compatible_with or [
        "@platforms//cpu:x86_64",
        "//:nova-boin",
    ]

    # we can separate out as much code as we can but there still
    # needs to be a _main entrypoint in srcs attached to
    # cc_binary for the rootpd. I do not know why but without it
    # the libentry assembly will not find the proper symbol. This
    # used to be specific to each rootpd in this repo but the new
    # macro handles this.
    ssrcs = (srcs or []) + [
        "//sys:boot.cc",
    ]
    cc_binary(
        name = name,
        deps = ddeps,
        srcs = ssrcs,
        target_compatible_with = target_compatible_with,
        **kwargs
    )

    # this is all x86_64 specific hooks but should be abstracted out
    # if/when we ever get an aarch cross-compiler setup
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

    native.genrule(
        name = name + "-grub-config",
        srcs = ["//sys/x86_64:grub.cfg.in"],
        outs = ["grub.cfg"],
        cmd = "sed 's,$$ROOT," + name + ",g' < $< > $@",
    )

    pkg_files(
        name = name + "-grub",
        srcs = [
            ":" + name + "-grub-config",
        ],
        attributes = pkg_attributes(
            group = "root",
            mode = "0551",
            owner = "root",
        ),
        prefix = "boot/grub",
    )
    pkg_files(
        name = name + "-nova",
        srcs = [
            ":" + name,
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
        name = name + "-nova-all",
        srcs = [
            ":" + name + "-grub",
            ":" + name + "-nova",
        ],
        visibility = ["//visibility:public"],
    )

    native.genrule(
        name = name + "-boot",
        srcs = [
            ":" + name + "-nova-all",
        ],
        outs = [name + "-boot.iso"],
        cmd = "$(locations //sys/x86_64:geniso) $@ $(locations :" + name + "-nova-all)",
        tools = ["//sys/x86_64:geniso"],
        visibility = ["//visibility:public"],
    )

    sh_binary(
        name = name + "-run",
        srcs = [name + "-run.sh"],
        args = [
            "$(location @ovmf-edk2//:ovmf_code)",
            "$(location @ovmf-edk2//:ovmf_vars)",
            "$(location :" + name + "-boot)",
        ],
        data = [
            ":" + name + "-boot",
            # we need the OVMF files because we want to use UEFI within
            # qemu. We dont need these in the ISO but they need to be
            # passed into QEMU.
            "@ovmf-edk2//:ovmf_code",
            "@ovmf-edk2//:ovmf_vars",
        ],
        target_compatible_with = [
        ],
    )

nova_rootpd = macro(
    inherit_attrs = native.cc_binary,
    implementation = _nova_rootpd_impl,
)

#
# nova_lib defines a wrapper for cc_library with some
# common settings and options.
#
def _nova_lib_impl(name, deps, **kwargs):
    ddeps = (deps or [
        "//lib",
        "//sys:syscalls",
    ])
    cc_library(
        name = name,
        deps = ddeps,
        **kwargs
    )

nova_lib = macro(
    inherit_attrs = native.cc_library,
    implementation = _nova_lib_impl,
)

#
# shared_lib wraps cc_library. unlike nova_lib,
# this one is for code shared between either
# local tools runnable in linux x86 or runnable
# in nova (for now this is NOT done, as we only
# have a single nova toolchain setup).
#
def _shared_lib_impl(name, **kwargs):
    cc_library(
        name = name,
        **kwargs
    )

shared_lib = macro(
    inherit_attrs = native.cc_library,
    implementation = _shared_lib_impl,
)
