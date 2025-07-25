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

#include "lib/types.hh"
#include "sys/nova/hip.hh"
#include "lib/IO.hh"

void repl(IO* io) {
  io->putstr("q -> quit\n");
  io->putstr("> ");
}

void pmain(uint32_t m2sig, ptr_t m2data, nova::HIP *hip, void *data) {
  IO *io = (IO *)data;
  io->setEcho(true);
  char buffer[256];
  while (1) {
    repl(io);
    io->readline(buffer);
    if (buffer[0] == 'q') {
      return;
    }
    io->putstr("? unknown\n");
  }
}
