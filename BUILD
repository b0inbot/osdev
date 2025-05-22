# file: BUILD
#
# The root build file, nothing really, but platform
# stuff that is currently unused.
#

package(default_visibility = ["//visibility:public"])

constraint_value(
    name = "nova-boin",
    constraint_setting = "@platforms//os",
    visibility = ["//visibility:public"],
)

# Using this as the platform everywhere will cause
# our genrules to fail due to no "non-nova" CC platform
# available.
platform(
    name = "x86_64_nova-boin",
    constraint_values = [
        ":nova-boin",
        "@platforms//cpu:x86_64",
    ],
)
