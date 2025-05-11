/* file: lib/math.hh
 *
 * Some useful math functions.
 */
#pragma once

#include "lib/types.hh"

inline uint64_t pow(uint64_t base, uint64_t exp) {
  uint64_t c = 1;
  for (uint64_t x = 0; x != exp; x++) {
    c = base * c;
  }
  return c;
}
