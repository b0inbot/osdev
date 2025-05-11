/* file: sys/x86_64/syscalls.cc
 *
 * The syscalls implementations. They are mostly a direct conversion
 * from the NOVA specification but some parts that are unclear in the
 * spec were figured out by trial and error and specified if possible.
 *
 */
#include "syscalls.hh"

extern "C" {
int _syscall(int a, int b, int c, int d, int e);
int _syscall2(int a, int b);
}

//
// not a direct syscall but a useful function. In qemu, it just
// shuts down so I know my program ran to this point and did not
// hit an error branch, were I panic and loop forever.
//
void sys_suspend_to_ram() {
  int rdi = 0x30c;
  _syscall2(rdi, 0);
}

int sys_create_pd(int sel, int parent, int type) {
  long rdi = (sel << 8) + (type << 4) + 0x2;
  long rsi = parent;
  return _syscall2(rdi, rsi);
}

int sys_ctrl_pd(uint64_t src, uint64_t dst, uint64_t ssb, uint64_t dsd, unsigned int ord,
                unsigned int pmm, int mad) {

  int rdi = (src << 8) + 0x7;
  int rsi = dst;
  int rdx = (ssb << 12) + ord;
  int rax = (dsd << 12) + pmm;
  int r8 = mad;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, rax, r8);
}

int sys_create_pt(int sel, int pd, int ec, void *ip) {
  long rdi = (sel << 8) + 0x5;
  long rsi = pd;
  long rdx = ec;
  long rax = (long)ip;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, rax, 0);
}

int sys_create_ec(int sel, int pd, int hvp, int evt, unsigned short cpu,
                  long sp, int flags) {
  // Where flags go in the NOVA spec are unclear but I believe
  // they are always shifted over 4 from the syscall ID.
  int rdi = (sel << 8) + (flags << 4) + HC_CREATE_EC;
  int rsi = pd;
  int rdx = hvp;
  int rax = (evt << 16) + cpu;
  int r8 = sp;
  // ------------ rdi, rsi, rdx, rcx, r8, r9
  return _syscall(rdi, rsi, rdx, rax, r8);
}
