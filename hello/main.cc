/* file: hello/main.cc
 *
 * hello _m2_main is called from the main assembly entrypoint
 * and hello becames the root program responsible for
 * spawning new programs, performing or delegating I/O, etc.
 *
 * For now we just suspend to ram, which turns off qemu and proves
 * that the binary loaded and invoked a system call.
 */

#include "sys/syscalls.hh"

extern "C" void _m2_main(unsigned long M2magic, unsigned char *M2,
                       unsigned char *HIP) {
  sys_suspend_to_ram();
}
