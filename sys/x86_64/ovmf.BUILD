# file: sys/x86_64/ovmf.BUILD
#
# The rules which pull out the required files from the debian package. These
# two files get passed straight into the run script for qemu.
#
package(default_visibility = ["//visibility:public"])

genrule(
    name = "ovmf_code",
    srcs = ["data.tar.xz"],
    outs = ["OVMF_CODE.fd"],
    cmd = "tar xf $< ./usr/share/OVMF/OVMF_CODE.fd; mv ./usr/share/OVMF/OVMF_CODE.fd $@",
)

genrule(
    name = "ovmf_vars",
    srcs = ["data.tar.xz"],
    outs = ["OVMF_VARS.fd"],
    cmd = "tar xf $< ./usr/share/OVMF/OVMF_VARS.fd; mv ./usr/share/OVMF/OVMF_VARS.fd $@",
)
