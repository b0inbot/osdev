# file: hello/BUILD
#
# The build file for the hellobinary, which is the entrypoint into
# the userspace right after NOVA is done.
#
# As noted, nothing in here or in main.cc are platform specific
# and we use bazel rules with select(...) expressions to delegate to
# the platform specific versions. I have no idea if this will be tenable
# longterm.
#

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "hello",
    # we can separate out as much code as we can but there still
    # needs to be a _main entrypoint in srcs attached to
    # cc_binary for the rootpd. I do not know why but without it
    # the libentry assembly will not find the proper symbol
    srcs = ["main.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//drv:uart",
        "//lib:headers",
        "//lib:math",
        "//sys:rootpd",
        "//sys/nova:headers",
        "//sys/nova:hypercalls",
    ],
)

# We just use empty to test the new cross compiler and its includes
cc_binary(
    name = "empty",
    # we can separate out as much code as we can but there still
    # needs to be a _main entrypoint in srcs attached to
    # cc_binary for the rootpd. I do not know why but without it
    # the libentry assembly will not find the proper symbol
    srcs = ["empty.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:headers",
        "//sys:rootpd",
    ],
)
