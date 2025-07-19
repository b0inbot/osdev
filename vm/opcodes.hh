#pragma once

namespace vm {

#define op_t constexpr const uint8_t

// sys
op_t OP_NOP = 0x01;
op_t OP_HLT = 0x02;

// math
op_t OP_ADD = 0x10;
op_t OP_SUB = 0x11;
op_t OP_MUL = 0x12;
op_t OP_DIV = 0x13;
op_t OP_INC = 0x14;
op_t OP_DEC = 0x15;
op_t OP_LSH = 0x16;
op_t OP_RSH = 0x17;
op_t OP_MOD = 0x18;

// mem
op_t OP_MOV = 0x20;

} // namespace vm
