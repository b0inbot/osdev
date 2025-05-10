load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

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
cc_binary(
    name = "hello",
    srcs = ["main.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:headers",
        "//sys:rootpd",
        "//sys/nova:hip",
    ],
)
