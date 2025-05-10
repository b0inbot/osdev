/* file: hello/main.cc
 *
 * hello _main is called from the assembly entrypoint
 * and hello becames the root program responsible for
 * spawning new programs, performing or delegating I/O, etc.
 *
 * For now we just check some magic values and then suspend to ram,
 * which turns off qemu and proves that the binary loaded and invoked a system
 * call. Otherwise, the system will spin and do nothing confirming a failure
 * somewhere.
 */

#include "lib/debug.hh"
#include "lib/types.hh"
#include "sys/nova/hip.hh"
#include "sys/syscalls.hh"

// since we calling _main from assembly, we need to tell the C++ compiler not to
// change the name on us. 'extern "C"' does that.
extern "C" void _main(uint32_t m2sig, ptr_t m2data, nova::HIP *hip) {
  // The multiboot2 magic signature is passed to us from NOVA who gets
  // it from the bootloader (grub in this repo).
  E_ASSERT(m2sig == 0x36d76289);

  // NOVA fills in the HIP (See sys/nova/hip.hh and the NOVA specification) and
  // its first few bytes spell out 'NOVA'. THe number below is the number
  // representation of NOVA, but importantly its backwards: 0x41 is 'A', 0x56 is
  // 'V'.. and so forth. A neat example of the little-endianess of x86_64.
  char *hip_ptr = (char *)(hip);
  E_ASSERT(*hip_ptr++ == 'N');
  E_ASSERT(*hip_ptr++ == 'O');
  E_ASSERT(*hip_ptr++ == 'V');
  E_ASSERT(*hip_ptr++ == 'A');
  E_ASSERT(hip->signature == 0x41564f4e);
  sys_suspend_to_ram();
}
