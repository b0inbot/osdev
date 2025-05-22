# file:  nova/x86_64/nova.BUILD
#
# Builds the NOVA kernel from the upstream sources. This just calls make directly for now.
#

package(default_visibility = ["//visibility:public"])

genrule(
    name = "kernel",
    srcs = glob(["**/*"]),
    outs = ["build-x86_64/x86_64-nova"],
    cmd = "set -ex; make PREFIX_x86_64=/opt/boin/osdev-cross/bin/x86_64-elf- -C external/+_repo_rules+nova-x86_64/; cp external/+_repo_rules+nova-x86_64/build-x86_64/x86_64-nova $@; rm -rf external/+_repo_rules+nova-x86_64/build-x86_64/;",
)
