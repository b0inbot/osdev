/* file: sys/nova/Hypercalls.hh
 *
 */
#pragma once

#include "lib/types.hh"

namespace nova {
template <typename T> struct Sel { uint64_t data; };
template <typename T> struct Space { uint64_t data; };
template <typename T> struct Position {
  uint64_t ref;
  uint64_t sb;
};

typedef void OBJ;
typedef void HST;
typedef void HST;
typedef void EVT;

}; // namespace nova

namespace nova::hypercalls {

enum class Result : uint64_t {
  Success,
  Timeout,
  Aborted,
  Overflow,
  BadHypercall,
  BadCapability,
  BadParameter,
  BadFeature,
  BadCPU,
  BadDevice,
  MemoryObject,
  MemoryCapability,
};

#define CREATE_EC_HOST_EXECUTION_CONTEXT 0b000
#define CREATE_EC_GUEST_EXECUTION_CONTEXT 0b001
#define CREATE_EC_LOCAL_THREAD 0b000
#define CREATE_EC_GLOBAL_THREAD 0b010
#define CREATE_EC_TIME_OFFSET 0b010
#define CREATE_EC_FPU 0b100

Result CreatePT(Sel<OBJ> sel, Sel<OBJ> pd, Sel<OBJ> ec, ptr_t ip);
Result CreateEC(Sel<OBJ> sel, Sel<OBJ> pd, Sel<HST> hvp, Sel<EVT> evt,
                uint64_t cpu, ptr_t sp, uint8_t flags);
Result CreateSC(Sel<OBJ> sel, Sel<OBJ> pd, Sel<OBJ> ec, uint64_t cos);

Result ControlPD(Position<OBJ> src, Position<OBJ> dst, uint64_t count);

}; // namespace nova::hypercalls

const char *to_string(nova::hypercalls::Result &);
const char *to_desc(nova::hypercalls::Result &);
