"""
"""

load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl", "feature", "flag_group", "flag_set", "tool_path")
load("@rules_cc//cc/common:cc_common.bzl", "cc_common")

all_compile_actions = [
    ACTION_NAMES.c_compile,
    ACTION_NAMES.cpp_compile,
]

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _impl(ctx):
    tool_paths = [
        tool_path(
            name = "gcc",
            path = "/opt/boin/osdev-cross/bin/x86_64-elf-gcc",
        ),
        tool_path(
            name = "ld",
            path = "/opt/boin/osdev-cross/bin/x86_64-elf-ld",
        ),
        tool_path(
            name = "ar",
            path = "/opt/boin/osdev-cross/bin/x86_64-elf-ar",
        ),
        tool_path(
            name = "cpp",
            path = "/bin/false",
        ),
        tool_path(
            name = "gcov",
            path = "/bin/false",
        ),
        tool_path(
            name = "nm",
            path = "/bin/false",
        ),
        tool_path(
            name = "objdump",
            path = "/bin/false",
        ),
        tool_path(
            name = "strip",
            path = "/bin/false",
        ),
    ]

    features = [
        feature(
            name = "default_compile_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_compile_actions,
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-fno-exceptions",
                                "-fno-rtti",
                                #TODO: this should be resolved better
                                "-I",
                                "external/+_repo_rules+dietlibc-x86_64/include/",
                            ],
                        ),
                    ]),
                ),
            ],
        ),
        feature(
            name = "default_linker_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-fno-exceptions",
                                "-fno-rtti",
                                # PLACEHOLDER
                                "-DNOVA",
                            ],
                        ),
                    ]),
                ),
            ],
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        cxx_builtin_include_directories = [
            "/opt/boin/osdev-cross/include/",
            "/opt/boin/osdev-cross/lib/gcc/x86_64-elf/15.1.0/include",
        ],
        features = features,
        toolchain_identifier = "gcc-nova-toolchain",
        host_system_name = "local",
        target_system_name = "nova",
        target_cpu = "x86_64",
        target_libc = "dietlibc",
        compiler = "gcc",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {
        "dietlibc": attr.label(),
    },
    provides = [CcToolchainConfigInfo],
)
