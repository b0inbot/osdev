# file: hello/BUILD
#
# The build file for the hellobinary, which is the entry point into
# the userspace right after NOVA is done.
#
# As noted, nothing in here or in main.cc are platform specific
# and we use bazel rules with select(...) expressions to delegate to
# the platform specific versions. I have no idea if this will be tenable
# longterm. This used to be more complex but now we have abstracted it
# into a macro.
#

load("//:defs.bzl", "nova_rootpd")

exports_files(["hello-run.sh"])

nova_rootpd(
    name = "hello",
    srcs = ["main.cc"],
    visibility = ["//visibility:public"],
    deps = [],
)
