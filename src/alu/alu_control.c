/**
 * @file alu_control.c
 * @brief ALU control logic for RISC-V instructions.
 *
 * @author eliorodr2104
 * @date 02/06/25
 *
 * This file contains the implementation of the ALU control logic that determines
 * the ALU operation based on the opcode, funct3, and funct7 fields of RISC-V instructions.
 *
 */

#include "alu_control.h"

/**
 * @brief Get the ALU operation bits and enum.
 *
 * @param alu_op The opcode of the instruction.
 * @param funz3 The funct3 field of the instruction.
 * @param funz7_bit30 The 30th bit of the funct7 field.
 * @param alu_tuple Contains alu operation information
 */
void get_alu_operation(
    const uint8_t   alu_op,
    const uint8_t   funz3,
    const uint8_t   funz7_bit30,
          AluTuple* alu_tuple

) {

    // Special case's
    switch (alu_op) {
        case 0x03: // Load
        case 0x17: // AUIPC
        case 0x6F: // JAL
            alu_tuple->enumOperation = ALU_ADD;
            alu_tuple->operation = 0x2;
            return;

        case 0x37: // LUI
        case 0x73: // ECALL
            alu_tuple->enumOperation = ALU_SKIP;
            alu_tuple->operation = 0xE;
            return;

        case 0x33: // R-type
        case 0x13: // I-type ALU
        case 0x67: // JALR
            // Continue to the function-specific logic
            break;

        default:
            alu_tuple->enumOperation = ALU_UNKNOWN;
            alu_tuple->operation = 0xF;
            return;
    }

    // Default for unknown funct3
    alu_tuple->enumOperation = ALU_UNKNOWN;
    alu_tuple->operation = 0xF;

    // Process based on funct3
    switch (funz3) {
        case 0x0: // ADD/SUB/ADDI/JALR
            if (alu_op == 0x33 && funz7_bit30) {
                alu_tuple->enumOperation = ALU_SUB;
                alu_tuple->operation = 0xA;
            } else {
                alu_tuple->enumOperation = ALU_ADD;
                alu_tuple->operation = 0x2;
            }
            break;

        case 0x1: // SLL/SLLI
            alu_tuple->enumOperation = ALU_SLL;
            alu_tuple->operation = 0x4;
            break;

        case 0x2: // SLT/SLTI
            alu_tuple->enumOperation = ALU_SLT;
            alu_tuple->operation = 0xB;
            break;

        case 0x4: // XOR/XORI
            alu_tuple->enumOperation = ALU_XOR;
            alu_tuple->operation = 0x7;
            break;

        case 0x5: // SRL/SRA/SRLI/SRAI
            if (funz7_bit30) {
                alu_tuple->enumOperation = ALU_SRA;
                alu_tuple->operation = 0x6;
            } else {
                alu_tuple->enumOperation = ALU_SRL;
                alu_tuple->operation = 0x5;
            }
            break;

        case 0x6: // OR/ORI
            alu_tuple->enumOperation = ALU_OR;
            alu_tuple->operation = 0x1;
            break;

        case 0x7: // AND/ANDI
            alu_tuple->enumOperation = ALU_AND;
            alu_tuple->operation = 0x0;
            break;

        default:
            alu_tuple->enumOperation = ALU_UNKNOWN;
            alu_tuple->operation = 0xF;
            break;
    }

}