/* file: hello/main.cc
 *
 * hello _m2_main is called from the main assembly entrypoint
 * and hello becames the root program responsible for
 * spawning new programs, performing or delegating I/O, etc.
 *
 * For now we just suspend to ram, which turns off qemu and proves
 * that the binary loaded and invoked a system call.
 */

#include "lib/types.hh"
#include "sys/syscalls.hh"

extern "C" void _m2_main(uint64_t M2magic, ptr_t M2, ptr_t HIP) {
  sys_suspend_to_ram();
}
