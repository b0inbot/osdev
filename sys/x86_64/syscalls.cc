/* file: sys/x86_64/syscalls.cc
 *
 * The syscalls implementations. They are mostly a direct conversion
 * from the NOVA specification but some parts that are unclear in the
 * spec were figured out by trial and error and specified if possible.
 *
 */
#include "sys/x86_64/syscalls.hh"

extern "C" {
uint64_t _syscall(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e);
uint64_t _syscall2(uint64_t a, uint64_t b);
}

//
// not a direct syscall but a useful function. In qemu, it just
// shuts down so I know my program ran to this point and did not
// hit an error branch, were I panic and loop forever.
//
[[noreturn]] void sys_suspend_to_ram() {
  int rdi = 0x30c;
  _syscall2(rdi, 0);
  for(;;) {}
}

uint64_t sys_create_pd(uint64_t sel, uint64_t parent, uint64_t type) {
  long rdi = (sel << 8) + (type << 4) + 0x2;
  long rsi = parent;
  return _syscall2(rdi, rsi);
}

uint64_t sys_ctrl_pd(uint64_t src, uint64_t dst, uint64_t ssb, uint64_t dsd,
                     uint64_t ord, uint64_t pmm, uint64_t mad) {

  int rdi = (src << 8) + 0x7;
  int rsi = dst;
  int rdx = (ssb << 12) + ord;
  int rax = (dsd << 12) + pmm;
  int r8 = mad;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, rax, r8);
}

uint64_t sys_create_pt(uint64_t sel, uint64_t pd, uint64_t ec, ptr_t ip) {
  long rdi = (sel << 8) + 0x5;
  long rsi = pd;
  long rdx = ec;
  long rax = (long)ip;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, rax, 0);
}

uint64_t sys_create_ec(uint64_t sel, uint64_t pd, uint64_t hvp, uint64_t evt,
                       unsigned short cpu, ptr_t sp, uint8_t flags) {
  // Where flags go in the NOVA spec are unclear but I believe
  // they are always shifted over 4 from the syscall ID.
  uint64_t rdi = (sel << 8) + (flags << 4) + HC_CREATE_EC;
  uint64_t rsi = pd;
  uint64_t rdx = hvp;
  uint64_t rax = (evt << 16) + cpu;
  uint64_t r8 = (uint64_t)sp;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, rax, r8);
}

uint64_t sys_create_sc(uint64_t sel, uint64_t pd, uint64_t ec, uint64_t scd) {
  uint64_t rdi = (sel << 8) + HC_CREATE_SC;
  uint64_t rsi = pd;
  uint64_t rdx = ec;
  uint64_t rax = scd;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, rax, 0);
}

uint64_t sys_ctrl_sc(uint64_t sc, uint64_t *ptr) {
  uint64_t rdi = (sc << 8) + HC_CONTROL_SC;
  return _syscall2(rdi, 0);
  // TODO: Read from rsi and populate ptr
}

uint64_t sys_ctrl_pt(uint64_t pt, uint64_t pid, uint64_t mtd) {
  uint64_t rdi = (pt << 8) + HC_CONTROL_PT;
  uint64_t rsi = pid;
  uint64_t rdx = mtd;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, 0, 0);
}

uint64_t sys_ipc_call(uint64_t sel, uint64_t *mtd) {
  // TODO: timeout as part of flags
  uint64_t rdi = (sel << 8) + HC_CONTROL_SC;
  uint64_t rsi = *mtd;
  return _syscall2(rdi, rsi);
  // TODO: Read from rsi and populate ptr
}
