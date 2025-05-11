/* file: hello/main.cc
 *
 * hello _main is called from the assembly entrypoint
 * and hello becames the root program responsible for
 * spawning new programs, performing or delegating I/O, etc.
 *
 * We perform a handful of hypercalls in order to grab the UART. Once
 * we do that, we shutdown.
 */

#define IGNORED(x) (x)

#include "lib/debug.hh"
#include "lib/math.hh"
#include "lib/types.hh"

#include "sys/nova/ObjectSpace.hh"
#include "sys/nova/hip.hh"

#include "drv/uart.hh"
#include "sys/syscalls.hh"

bool strequal(char *l, const char *r, uint32_t len) {
  for (uint32_t x = 0; x != len; x++) {
    if (*l++ != *r++)
      return false;
  }
  return true;
}

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
  //
  // NOTE: Some commits of this repo had this check fail because the way the
  // binary was linked was causing NOVA to call _main directly, ignoring the
  // _entry that moves HIP into argument 3 here. So this is an additional check
  // that we were called by our entrypoint and not called from anywhere else.
  char *hip_ptr = (char *)(hip);
  E_ASSERT(strequal(hip_ptr, "NOVA", 4));
  E_ASSERT(hip->signature == 0x41564f4e);

  // All the magic numbers here are described in the NOVA specification
  // and are all relative to the values given to us in the HIP.
  //
  auto top = ipow(2, hip->sbw.obj);
  nova::CurrentObjectSpace root{top - 2, top, top - 6};
  auto novaOS = nova::ObjectSpace{root.rel(-1)};

  // When the kernel receives a hypercall from us, the references to the
  // objects are all relative to the ObjectSpace attached to us. So if
  // we want to reference kernel objects that do not belong to us, we have to
  // copy them over with a control hypercall. We copy them over and they
  // get a different offset in *our* space than they have in the *source* space.
  //
  // Example: consoleSemaphore is described as -1 but when we copy it from
  // Nova Object Space to Root Object Space our destination is -32 so we
  // end up at -32-1. If we want to use the console semaphore, our hypercalls
  // must use -32-1 and (but only if we do the ctrl hypercall first to copy it
  // there).
  //
  // How do we know it works? Well, we get the two PIOs below (novaPIO and
  // rootPIO) and make hypercalls using them. If they didnt get copied in we
  // would crash. additionally, the PIO objects help open up the UART which,
  // again, if we invoked without the correct hypercalls we would crash.
  //
  nova::TakenRef novaObjs;
  {
    uint8_t amount = 16;
    uint64_t dest = top - 32 * 2;
    long dsb = dest - amount;
    long ord = log2(amount);
    long ssb = top - amount;
    // just mark TAKE and GRANT as allowed which will allow our PIO objects to
    // be controled. The full range of capability objects we are copying over
    // are all of different types so I'm unsure how to tweak the bits properly.
    uint8_t pmm = 0b00011;
    novaObjs = novaOS.take(dest, ssb, dsb, ord, pmm, IGNORED(0));
    if (!novaObjs.valid()) {
      return;
    }
  }

  auto novaPIO = novaObjs.rel(-4);
  auto rootPIO = novaObjs.rel(-8);

  // Our current process has NO permissions to do PIO but the novaPIO space
  // has permissions and we have permissions to take from novaPIO.
  // So copy from nova to our root.
  {
    //
    // Somehow COM1 at 0x3f8 is the sum of 2^N where N -> 9 to 3 so we can copy
    // at the very start and only copy what we need (2^ord where ord=3 so 8).
    // If that wasn't the case, the byte-order aligned math would not work out
    // and we would have to copy more than we need.
    //
    // dsb is destination and it makes 0x3f8 map to itself and since
    // dsb and ssb must be the same if we are working on the PIO space we
    // cant do any sort of offsets or mapping.
    //
    int amount = 8;
    long ord = log2(amount);
    long dsb = 512 + 256 + 128 + 64 + 32 + 16 + 8;
    int pmm = 0b1; // PIOs only use 1 bit (allow or not allow I/O)
    int ret = rootPIO.control(novaPIO.ident(), dsb, dsb, ord, pmm, IGNORED(0));
    if (ret != 0) {
      return;
    }
  }

  // Now that we have permissions to access the UART we can undo our take from
  // before. We do this by runnign the same syscall but
  // marking the pmm bits as 0 which marks the destination objects as null
  // capability.
  //
  // We could always untake a smaller or bigger subset of what we copied before.
  novaObjs.untake();

  // Initialize and use the UART.
  Uart COM1(COM1_PORT);
  if (!COM1.init()) {
    return;
  }
  COM1.putstr("MAIN: UART Ready\n");

  // Reconfirm taking from PIO fails
  {
    long ord = 3;
    long dsb = 512 + 256 + 128 + 64 + 32 + 16 + 8;
    int pmm = 0b1; // PIOs only use 1 bit (allow or not allow I/O)
    int ret = rootPIO.control(novaPIO.ident(), dsb, dsb, ord, pmm, IGNORED(0));
    if (ret != 0) {
      COM1.putstr("MAIN: OK: Checking expected PIO perms\n");
    } else {
      COM1.putstr("MAIN: FAIL: Checking expected PIO perms\n");
    }
  }

  sys_suspend_to_ram();
}
