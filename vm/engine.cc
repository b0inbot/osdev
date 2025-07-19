#include "vm/engine.hh"

vm::Engine::Engine() {
  for (uint8_t x = 0; x != vm::RegCount; x++) {
    R[x] = 0;
  }
}

void vm::Engine::execute(uint8_t *buf) {}
