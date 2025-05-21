/* file: sys/nova/Hypercalls.cc
 */

#include "sys/nova/Hypercalls.hh"
#include "lib/math.hh"
#include "sys/syscalls.hh"

using namespace nova;
using namespace nova::hypercalls;

const char *to_string(Result &res) {
  switch (res) {
  case Result::Success:
    return "Success";
  case Result::Timeout:
    return "Timeout";
  case Result::Aborted:
    return "Aborted";
  case Result::Overflow:
    return "Overflow";
  case Result::BadHypercall:
    return "BadHypercall";
  case Result::BadCapability:
    return "BadCapability";
  case Result::BadParameter:
    return "BadParameter";
  case Result::BadFeature:
    return "BadFeature";
  case Result::BadCPU:
    return "BadCPU";
  case Result::BadDevice:
    return "BadDevive";
  case Result::MemoryObject:
    return "MemoryObject";
  case Result::MemoryCapability:
    return "MemoryCapability";
  }
  return "Unknown";
}

const char *to_desc(Result &res) {
  switch (res) {
  case Result::MemoryObject:
    return "Insufficient Memory (Object Creation)";
  case Result::MemoryCapability:
    return "Insufficient Memory (Capability Creation)";
  default:
    return to_string(res);
  }
}

namespace nova::hypercalls {

Result CreatePT(Sel<OBJ> sel, Sel<OBJ> pd, Sel<OBJ> ec, ptr_t ip) {
  return Result{(uint64_t)sys_create_pt(sel.data, pd.data, ec.data, ip)};
}

Result CreateEC(Sel<OBJ> sel, Sel<OBJ> pd, Sel<HST> hvp, Sel<EVT> evt,
                uint64_t cpu, ptr_t sp, uint8_t flags) {
  return Result{(uint64_t)sys_create_ec(sel.data, pd.data, hvp.data, evt.data,
                                        cpu, sp, flags)};
}

Result CreateSC(Sel<OBJ> sel, Sel<OBJ> pd, Sel<OBJ> ec, uint64_t cos) {
  return Result{(uint64_t)sys_create_sc(sel.data, pd.data, ec.data, cos)};
}

Result ControlPD(Position<OBJ> src, Position<OBJ> dst, uint64_t count,
                 uint8_t pmm) {
  if ((count & (count - 1)) == 0) {
    return Result::BadParameter;
  }
  uint32_t ord = log2(count);
  return Result{
      sys_ctrl_pd(src.ref, dst.ref, src.sb, dst.sb, ord, pmm, 0),
  };
}

} // namespace nova::hypercalls
