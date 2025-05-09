/* file: sys/syscalls.hh
 *
 * Syscall header which is architecture agnostic.
 */

#pragma once

#ifdef X86_64
#include "sys/x86_64/syscalls.hh"
#endif
