/* file: lib/debug.hh
 *
 * Some debug macros.
 *
 */
#pragma once

// This assert just loops since we have no video or audio or console to
// report any invalid conditions. We consider 'looping forever' an invalid
// condition and hitting the end of the hello prpgram
// and shutting down a valid condition (for now). There will be a version of
// this assert that will write to the COM port when we get access to it.
#define E_ASSERT(x)                                                            \
  do {                                                                         \
    if (!(x)) {                                                                \
      for (;;) {                                                               \
      }                                                                        \
    }                                                                          \
  } while (0)
