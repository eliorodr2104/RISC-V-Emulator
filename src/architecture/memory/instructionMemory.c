//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "instructionMemory.h"
#include "cpu.h"
#include "tools.h"

uint32_t instructions[MAX_INSTRUCTIONS] = {
    // 0xFFF34293  // xori t0, t1, -1
       0x40135293  // srai t0, t1, 1
    // 0x00135293  // srli t0, t1, 1
    // 0x00131293  // slli t0, t1, 1
    // 0x407352B3  // sra  t0, t1, t2
    // 0x007352B3  // srl  t0, t1, t2
    // 0x007312B3  // sll  t0, t1, t2
    // 0x00430293  // addi t0, t1, 4
    // 0x007302B3, // add  t0, t1, t2
    // 0x407302B3, // sub  t0, t1, t2
    // 0x000002B3, // sub  t0, zero, zero
};

uint32_t fetchInstruction(Cpu* cpu) {

    if (!cpu) return 0;

    if (cpu->pc >= MAX_INSTRUCTIONS) return 0;

    return instructions[cpu->pc];
}

// Decodifica istruzione RISC-V
DecodedInstruction decodeInstruction(const uint32_t instruction) {
    DecodedInstruction decoded = { 0 };

    decoded.opcode      = extractBits(instruction, 0, 6);    // [6-0]
    decoded.rd          = extractBits(instruction, 7, 11);   // [11-7]
    decoded.funct3      = extractBits(instruction, 12, 14);  // [14-12]
    decoded.rs1         = extractBits(instruction, 15, 19);  // [19-15]
    decoded.rs2         = extractBits(instruction, 20, 24);  // [24-20]
    decoded.funct7Bit30 = extractBits(instruction, 30, 30);  // [30]

    // Decodifica immediato basato sull'opcode (tipo di istruzione)
    switch (decoded.opcode) {
            // Tipo I (es. ADDI, LW)
        case 0x13:
            // LOAD
        case 0x03:
            decoded.immediate = signExtend(extractBits(instruction, 20, 31), 12);
            break;

            // Tipo S
        case 0x23:
            {
                const uint32_t imm_11_5 = extractBits(instruction, 25, 31);
                const uint32_t imm_4_0  = extractBits(instruction, 7, 11);
                const uint32_t imm_s    = (imm_11_5 << 5) | imm_4_0;
                decoded.immediate       = signExtend(imm_s, 12);
            }
            break;

            // Tipo B
        case 0x67:
            {
                const uint32_t imm_12   = extractBits(instruction, 31, 31);
                const uint32_t imm_11   = extractBits(instruction, 7, 7);
                const uint32_t imm_10_5 = extractBits(instruction, 25, 30);
                const uint32_t imm_4_1  = extractBits(instruction, 8, 11);
                const uint32_t imm_b    = (imm_12 << 12) | (imm_11 << 11) | (imm_10_5 << 5) | (imm_4_1 << 1);
                decoded.immediate       = signExtend(imm_b, 13);
            }
            break;

            // Tipo U
        case 0x37:
            decoded.immediate = (int32_t)(instruction & 0xFFFFF000);
            break;

            // Tipo J
        case 0x6F:
            {
                const uint32_t imm_20    = extractBits(instruction, 31, 31);
                const uint32_t imm_19_12 = extractBits(instruction, 12, 19);
                const uint32_t imm_11    = extractBits(instruction, 20, 20);
                const uint32_t imm_10_1  = extractBits(instruction, 21, 30);
                const uint32_t imm_j     = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);
                      decoded.immediate  = signExtend(imm_j, 21);
            }
            break;

            // Tipo R
        case 0x33:
        default:
            decoded.immediate = 0;
            break;
    }

    return decoded;
}
