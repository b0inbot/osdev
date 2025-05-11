/* file: sys/nova/ObjectSpace.hh
 *
 * A bunch of classes and methods for working with the NOVA
 * kernel objects and making hypercalls in an API that is
 * adequitely high level. For now we just have enough
 * to get the UART.
 *
 * The whole file needs to be separated out.
 */
#pragma once

#include "lib/types.hh"
#include "sys/syscalls.hh"

namespace nova {

class CurrentObjectSpace;

/*
 * A Reference to a kernel object within NOVA, defined in relation to the
 * CurreentObjectSpace.
 *
 * We have a valid flag but that does not actually validate if the kernel
 * believes this is valid. It is merely for user-space code to say "did
 * we fill this ref in or not?".
 *
 * We can additionally build an invalid ref that points to a real object space
 * and that would be considered a "Null" object.
 */
class Ref {
private:
  uint64_t _id;
  CurrentObjectSpace *_parent;
  bool _valid;

public:
  Ref() : _valid(false) {}
  Ref(uint64_t id, CurrentObjectSpace *parent)
      : _id(id), _parent(parent), _valid(true) {}
  Ref(uint64_t id, CurrentObjectSpace *parent, bool valid)
      : _id(id), _parent(parent), _valid(valid) {}

  inline uint64_t ident() { return _id; }
  inline CurrentObjectSpace *parent() { return _parent; }
  inline bool valid() { return _valid; }

  // this is a pretty low level API exposed here that we would preferably design
  // another way and place it elsewhere.
  inline int control(int srcPD, int ssb, int dsb, uint32_t ord, uint32_t pmm,
                     int mad) {
    return sys_ctrl_pd(srcPD, _id, ssb, dsb, ord, pmm, mad);
  }

  inline Ref rel(int64_t offset, uint8_t multiplier = 1) {
    return Ref{_id + (offset * multiplier), _parent};
  }
};

/*
 * The object representing the object space of the current process. Used
 * as the core object to build refs to other objects, spaces, and capabilities.
 */
class CurrentObjectSpace {
private:
  uint64_t _id;
  uint64_t _top;
  uint64_t _next;

public:
  CurrentObjectSpace(uint64_t id, uint64_t top, uint64_t next)
      : _id(id), _top(top), _next(next) {}

  // The NOVA specification describes kernel objects as an offset
  // from the calculated top number, so we have a relative reference
  // lookup.
  //
  // Since relative can be negative or positive we support a multiplier so
  // we can get max
  inline Ref rel(int64_t offset, uint8_t multiplier = 1) {
    return Ref{_top + (offset * multiplier), this};
  }

  inline Ref abs(uint64_t id) { return Ref{id, this}; }

  inline uint64_t ident() { return _id; }
};

/**
 * A taken reference is a reference to some object that has been taken
 * from another object space using a hypercall.
 *
 * It is merely a Ref object paired with the hypercall information about
 * how it was created, with methods to undo the take so that we can treat
 * references to other objects as transient, possibly even running untake
 * in a deconstructor.
 */
class TakenRef {
private:
  Ref _ref;
  int _srcPD;
  int _id;
  int _ssb;
  int _dsb;
  int _ord;

public:
  inline bool valid() { return _ref.valid(); }
  inline Ref ref() { return _ref; }

  inline Ref rel(int64_t offset, uint8_t multiplier = 1) {
    return _ref.rel(offset, multiplier);
  }
  TakenRef() {}
  TakenRef(Ref ref, int srcPD, int id, int ssb, int dsb, int ord)
      : _ref(ref), _srcPD(srcPD), _id(id), _ssb(ssb), _dsb(dsb), _ord(ord) {}

  inline void untake() { sys_ctrl_pd(_srcPD, _id, _ssb, _dsb, _ord, 0, 0); }
};

/*
 * Object which points to an external object space that we are able to take from
 * or grant to.
 */
class ObjectSpace {
private:
  Ref _ref;

public:
  ObjectSpace(Ref ref) : _ref(ref) {}

  // This is an improvement on the above control method, but the math
  // of ssb, dsd, and ord is not fun yet.
  inline TakenRef take(uint64_t newTop, uint64_t ssb, uint64_t dsb, uint32_t ord, uint32_t pmm,
                       int mad) {
    auto srcPD = _ref.ident();
    auto id = _ref.parent()->ident();
    auto status = sys_ctrl_pd(srcPD, id, ssb, dsb, ord, pmm, mad);
    if (status == 0) {
      return TakenRef{Ref{newTop, _ref.parent()}, srcPD, id, ssb, dsb, ord};
    } else {
      return TakenRef{};
    }
  }
};

} // namespace nova
