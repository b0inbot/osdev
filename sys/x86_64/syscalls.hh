/* file: sys/x86_64/syscall.hh
 *
 * C interface to the NOVA hypercalls. This is a straightforward
 * and overly simple conversion of hypercalls to C and most
 * of it probably does not need to be architecture-specific.
 */
#pragma once

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

int sys_create_pd(int sel, int pd, int type);
int sys_create_pt(int sel, int pd, int ec, void *ip);
int sys_ctrl_pd(int src, int dst, int ssb, int dsd, unsigned int ord,
                unsigned int pmm, int mad);
int sys_create_ec(int sel, int pd, int hvp, int evt, unsigned short cpu,
                  void *sp, int flags);

void sys_suspend_to_ram();
