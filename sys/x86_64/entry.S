# vim: softtabstop=4 expandtab
# file: sys/x86_64/entry.S
#
# Simple assembly entrypoint that moves the NOVA and Multiboot2
# details into the C-argument registers before calling _main.
#
# Not necessarily required as you could jump straight into _main
# and read the registers directly.
#
.global _start
_start:
    // rip: pdroot ELF image entry point
    // rsp: hypervisor information page(HIP)
    // rdi: EAX at boot time (multiboot v2 magic)
    // rsi: EBS at book time (multiboot v2 data)
    mov %rsp, %rdx // put the HIP into arg3
    # _main(%rdi, %rsi, %rdx)
    # _main(multiboot_magic, multiboot_ptr, hip_ptr)
    call _main

# loop forever in case we get here
_l:
    jmp _l

#
# Generic syscall functions which just forward the given params
# over and returns status.
#
# The NOVA system call interface almost directly matches the
# C-call convention except that RCX is replaced with RAX
# so when we need it we just move it over. All the smarter
# parameter building logic is written in C++ in the syscall package.
#
# We have a version of syscall (syscall2) that skips this RCX to RAX step
# in cases where the syscall is too simple to need a 3rd parameter.
#
.global _syscall
_syscall:
   push %rbp
   mov %rsp, %rbp
   mov %rcx, %rax
   syscall
   mov %rdi, %rax // put status in rax
   leave
   ret

.global _syscall2
_syscall2:
   push %rbp
   mov %rsp, %rbp
   syscall
   mov %rdi, %rax // put status in rax
   leave
   ret


.global data
data:
