"""
Macros to wrap our usage of cc_* rules so that we can
tweak various options project-wide.
"""

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

# the rootpd is the main userspace entrypoint. It
# requires a _main which is in //sys:boot.cc and
# rootpd expects a pmain function definition.
#
def _nova_rootpd_impl(name, srcs, deps, **kwargs):
    ddeps = (deps or []) + [
        "//drv:uart",
        "//lib:headers",
        "//lib:math",
        "//sys:rootpd",
        "//sys/nova:headers",
        "//sys/nova:hypercalls",
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
        **kwargs
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
        "//lib:headers",
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
