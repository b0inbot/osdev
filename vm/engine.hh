#pragma once

#include "lib/types.hh"

namespace vm {

constexpr const uint8_t RegCount = 12;

struct Engine {
  uint64_t R[RegCount];
  Engine();
  void execute(uint8_t *buf);
};

} // namespace vm
