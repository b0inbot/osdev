/* file: hello/main.cc
 *
 * hello pmain is called from a shared hardware-specific boot
 * entrypoint under sys/$arch/boot.cc
 *
 * hello becames the root program responsible for
 * spawning new programs, performing or delegating I/O, etc.
 *
 */

#define IGNORED(x) (x)

#include "lib/debug.hh"
#include "lib/math.hh"
#include "lib/types.hh"

#include "sys/nova/Hypercalls.hh"
#include "sys/nova/ObjectSpace.hh"
#include "sys/nova/hip.hh"

#include "drv/uart.hh"
#include "sys/syscalls.hh"

void pmain(uint32_t m2sig, ptr_t m2data, nova::HIP *hip, void *data) {
  Uart *COM1 = (Uart *)data;
  COM1->putstr("MAIN: In pmain\n");
}
