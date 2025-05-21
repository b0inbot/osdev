/* file: sys/x86_64/syscall.hh
 *
 * C interface to the NOVA hypercalls. This is a straightforward
 * and overly simple conversion of hypercalls to C and most
 * of it probably does not need to be architecture-specific.
 */
#pragma once

#include "lib/types.hh"

#define OP_PD 0x0
#define OP_OS 0x1
#define OP_HS 0x2
#define OP_GS 0x3
#define OP_DMA 0x4
#define OP_PIO 0x5
#define OP_MSR 0x6

#define EC_HOST_CTX 0x0
#define EC_GUEST_CTX 0x1
#define EC_FPU 0x1

#define HC_IPC_CALL 0x0
#define HC_IPC_REPLY 0x1
#define HC_CREATE_PD 0x2
#define HC_CREATE_EC 0x3
#define HC_CREATE_SC 0x4
#define HC_CREATE_PT 0x5
#define HC_CREATE_SM 0x6
#define HC_CONTROL_PD 0x7
#define HC_CONTROL_EC 0x8
#define HC_CONTROL_SC 0x9
#define HC_CONTROL_PT 0xA
#define HC_CONTROL_SM 0xB
#define HC_CONTROL_HW 0xC
#define HC_ASSIGN_INT 0xD
#define HC_ASSIGN_DEV 0xE

#define SUCCESS 0x0
#define TIMEOUT 0x1
#define ABORTED 0x2
#define OVRFLOW 0x3
#define BAD_HYP 0x4
#define BAD_CAP 0x5
#define BAD_PAR 0x6
#define BAD_FTR 0x7
#define BAD_CPU 0x8
#define BAD_DEV 0x9
#define MEM_OBJ 0xA
#define MEM_CAP 0xB

uint64_t sys_create_pd(uint64_t sel, uint64_t pd, uint64_t type);
uint64_t sys_create_pt(uint64_t sel, uint64_t pd, uint64_t ec, ptr_t ip);
uint64_t sys_create_ec(uint64_t sel, uint64_t pd, uint64_t hvp, uint64_t evt,
                       uint16_t cpu, ptr_t sp, uint8_t flags);
uint64_t sys_create_sc(uint64_t sel, uint64_t pd, uint64_t ec, uint64_t scd);

uint64_t sys_ctrl_pt(uint64_t pt, uint64_t pid, uint64_t mtd);

uint64_t sys_ctrl_pd(uint64_t src, uint64_t dst, uint64_t ssb, uint64_t dsd,
                     uint64_t ord, uint64_t pmm, uint64_t mad);
uint64_t sys_ctrl_sc(uint64_t sc, uint64_t *ptr);

uint64_t sys_ipc_call(uint64_t sel, uint64_t *mtd);

[[noreturn]] void sys_suspend_to_ram();
