# file: nova/BUILD
#
#
package(default_visibility = ["//visibility:public"])

alias(
    name = "kernel",
    actual = select({
        "@platforms//cpu:x86_64": "@nova-x86_64//:kernel",
        "//conditions:default": ":empty",
    }),
)
