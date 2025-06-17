/**
 * @file alu-control.c
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
 * @brief Get the ALU control operation based on the opcode, funct3, and funct7 bit 30.
 *
 * @param alu_op The opcode of the instruction.
 * @param funz3 The funct3 field of the instruction.
 * @param funz7_bit30 The 30th bit of the funct7 field.
 *
 * @return AluOp The ALU operation to be performed.
 */
AluOp get_alu_control(
    const uint8_t alu_op,
    const uint8_t funz3,
    const uint8_t funz7_bit30

) {

    // Check for the ALU operation based on the opcode and funct3
    switch (alu_op) {

        // This is an instruction Tipe-R
        case 0x33:

            // Control funz3 and determinate the ALU operation, in rare case use funz7
            switch (funz3) {
                case 0x0: return funz7_bit30 ? ALU_SUB : ALU_ADD;
                case 0x1: return ALU_SLL;
                case 0x2: return ALU_SLT;
                case 0x4: return ALU_XOR;
                case 0x5: return funz7_bit30 ? ALU_SRA : ALU_SRL;
                case 0x6: return ALU_OR;
                case 0x7: return ALU_AND;

                default:  return ALU_UNKNOWN;
            }

        // This is an instruction Tipe-I and special cose jalr
        case 0x67:
        case 0x13:

            // Control funz3 and determinate the ALU operation, in rare case use funz7
            switch (funz3) {
                case 0x0: return ALU_ADD;                          // addi or jalr
                case 0x1: return ALU_SLL;                          // slli
                case 0x4: return ALU_XOR;                          // xori
                case 0x5: return funz7_bit30 ? ALU_SRA : ALU_SRL;  // srai or srli
                case 0x6: return ALU_OR;                           // ori
                case 0x7: return ALU_AND;                          // andi

                default: return ALU_UNKNOWN;

            }

        // This is an instruction Tipe-J
        case 0x6F:
            return ALU_ADD;

        // This is an instruction Tipe-I, special case for ecall
        case 0x73:
            return ALU_SKIP;

        // This is an exception
        default:
            return ALU_UNKNOWN;
    }
}

/**
 * @brief Get the ALU operation bits based on the ALU operation enum.
 *
 * @param alu_op_enum The ALU operation enum.
 * @param operation Pointer to store the operation bits.
 */
void get_alu_operation_bits(
    const AluOp    alu_op_enum,
          uint8_t* operation
) {

    // Initialize the operation to an invalid value
    switch (alu_op_enum) {

        case ALU_ADD:
            *operation = 0x2; // 0b00010

        break;

        case ALU_SUB:
            *operation = 0xA; // 0b01010

        break;

        case ALU_AND:
            *operation = 0x0;

        break;

        case ALU_OR:
            *operation = 0x1;

        break;

        case ALU_SLT:
            *operation = 0xB; // 0b01011

        break;

        case ALU_SLL:
            *operation = 0x4; // 0b0010;

        break;

        case ALU_SRL:
            *operation = 0x5; // 0b00101

        break;

        case ALU_SRA:
            *operation = 0x6; // 0b00110

        break;

        case ALU_XOR:
            *operation = 0x7; // 0b00111

        break;

        case ALU_SKIP:
            *operation = 0xE; // 0b01110 -> This is a skip operation, not an ALU operation

        break;

        default:
            *operation = 0xF; // 0b1111 -> This is an invalid operation

        break;
    }
}
